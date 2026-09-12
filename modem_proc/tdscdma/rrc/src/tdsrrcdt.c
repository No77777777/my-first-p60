/*===========================================================================
                  R R C  D I R E C T  T R A N S F E R

DESCRIPTION
  This file contains the five procedures that deal with direct transfer:
  Initial Direct Transfer, Uplink Direct Transfer, Downlink Direct Transfer,
  Signalling Connection Release, and Signalling Connection Release Request.

EXTERNALIZED FUNCTIONS
  tdsrrcidt_procedure_event_handler
    This function is the event handler for all events that are sent to the
    Initial Direct Transfer procedure.

  tdsrrcudt_procedure_event_handler
    This function is the event handler for all events that are sent to the
    Uplink Direct Transfer procedure.

  tdsrrcddt_procedure_event_handler
    This function is the event handler for all events that are sent to the
    Downlink Direct Transfer procedure.

  tdsrrcscr_procedure_event_handler
    This function is the event handler for all events that are sent to the
    Signalling Connection Release procedure.

  tdsrrcscrr_procedure_event_handler
    This function is the event handler for all events that are sent to the
    Signalling Connection Release Request procedure.

  tdsrrcdt_init_procedure
    This function initializes substates of initial direct transfer,
    uplink direct transfer and downlink direct transfer procedures and other
    parameters to validate the sessions.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None.

Copyright (c) 2010-2012 by Qualcomm Technologies Incorporated.  
All Rights Reserved.QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcdt.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/03/13   hx     Buffering the DL DT message that arrives UE during SRNS reloc pending state on source cell.
06/17/12   hx     Fix memory allocate/free for ASN.1 structures.
10/27/11   hx     Up-Merged WCDMA CR: 289288
03/10/11   bj     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"

#include "tdsrrccmd_v.h"               /* RRC command definitions */
#include "tdsrrccmd.h"
#include "tdsrrcdt.h"
#include "tdsrrclcm.h"               /* Logical Channel Manager */
#include "tdsrrcmeas.h"              /* Meas Report */
#include "tdsrrcsend.h"              /* For sending a signalling message */
#include "msg.h"
#include "err.h"
#include "tdsrrcasn1.h"
#include "mmtask.h"
#ifdef FEATURE_CMI
#include "mmtask_v.h"
#endif
#include "tdsrrcscmgr.h"
#include "tdsrrccu.h"
#include "tdsrrcrce.h"
#include "tdsrrcnv.h"
#include "tdsrrcmcm.h"
#include "tdsrrcsmc.h"
#include "tdsrrclogging.h"
#include "tdsrrcccm.h"
#include "tdsrrcmisc.h"
/*
#include "tdsrrc_rr_types.h"
#include "rr_rrc_if.h"
*/
#include "tdsrrc_rr_if.h"
#include "rr_tdsrrc_if.h"

#include "tdsrrccspi.h"
#include "tdsrrccsp.h"
#ifdef FEATURE_TDSCDMA_SRNS_MO_CALL_DEFER
#include "tdsrrcrbreconfig.h"
#endif
#ifdef  FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
#include "tdsrrcmeasi.h"
#endif
#include "tdsrrcrcei.h"
#include "tdsrrcsibproc.h"

#include "tdsrrcdormancy.h"

#ifdef TEST_FRAMEWORK
#error code not present
#endif
#include "tdsrrcasn1util.h"
#include "tdsCFAParser_i.h"
#include "tdsrrccommon.h"
#include "tdsrrctmr.h"
#include "mm_as_types.h"
#include "mm.h"
#include "sm.h"

 /* boolean flag to read NV item and check to see if Fake Secured
    are enabled from QPST
  */
boolean    tdsnv_fake_security_enabled;     

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
boolean tdsis_scrr_in_progress = TRUE;
/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */

/* This variable holds all of the regional data for this procedure
 */
tdsrrcdt_int_data_type        tdsrrcdt_int_data;

/* This holds the establishment cause which CU/IDT send */
tdsrrcidt_est_cause_info_type tdsrrcidt_est_cause_info;

/* This variable holds substate for Initial Direct Transfer Procedure
 */
tdsrrcidt_substate_e_type     tdsrrcidt_substate;
/* This variable holds substate for Uplink Direct Transfer Procedure
 */
tdsrrcudt_substate_e_type     tdsrrcudt_substate;
/* This variable holds substate for Signalling Connection Release Request
 * Procedure */
tdsrrcscrr_substate_e_type    tdsrrcscrr_substate;


/* This struct holds the Initial Direct Transfer Procedure contents 
   which are saved from the OPEN_SESSION_REQ call
 */
tdsrrcidt_open_sess_req_type  tdsrrcidt_cs_saved_open_session_req;

tdsrrcidt_open_sess_req_type  tdsrrcidt_ps_saved_open_session_req;


/* This variable is used to save TDSRRC_DATA_REQ messages that are 
   supposed to be sent CS Domain */

tdsrrcudt_data_req_type  tdsrrcudt_cs_saved_data[MAX_NAS_MESSAGES_FOR_CS_DOMAIN];
/* This variable is used to save TDSRRC_DATA_REQ messages that are 
   supposed to be sent CS Domain */
tdsrrcudt_data_req_type  tdsrrcudt_ps_saved_data[TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN];

/* This variable is used to save RRC_DATA_IND messages that are supposed
   to be forwarded to NAS. */
tdsrrcddt_data_ind_type tdsrrcddt_saved_data[MAX_NAS_MESSAGES_FOR_CS_DOMAIN +
                                       TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN];


/* These variable saves close session request
  for either PS or CS call.
*/

tdsrrcscrr_close_sess_req_type   tdsrrcscrr_saved_cs_close_session_req;
tdsrrcscrr_close_sess_req_type   tdsrrcscrr_saved_ps_close_session_req;

#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
extern tdsrrcrce_csfb_status_e_type tdsrrc_csfb_extended_status;
#endif
#ifdef FEATURE_TDSCDMA_SRNS_MO_CALL_DEFER
/*added for SRNS reallocation process*/
extern tdsrrcrbrc_int_data_type tdsrrc_rbrc;
#endif

static boolean tdsrrc_hold_ps_tmr_expired=TRUE;
extern tdsrrc_cs_domain_error_count_type tdsrrc_cs_domain_error;
extern sys_proc_type_e_type tdsrrc_proc_type;

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */



/*===========================================================================
** -----------------------------------------------------------------------------------
**-- ----------NON-Demand paging section Srart--------------------------------------------
** -----------------------------------------------------------------------------------
  This section will have code section that will not be demand paged. Function which should be in this section are
  -RRC functions that are called by L1/L2 which are higher priority then RRC
  -RRC functions that are called in ISR context or RTOS timer call back
  -RRC functions that indirectly gets called by L1/L2 /ISR/timer call back   
  For example: Fun1() gets called by L1. Fun2() is called by Fun1(). Now both  Fun1() & Fun2() should be NON demand paged, 
  so both should be added in this section  
  Usage of pragma 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void foo(void)
  {
    function body here
  }
        __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
  If you have another function to be added in the same section then again it needs to be wrapped with these pragma 
  for example:- function bar() is just below function foo() then function bar() needs to be written like this ? 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void bar(void)
  {
    function body here
  }
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
** ----------------------------------------------------------------------- */



/*===========================================================================

FUNCTION tdsrrcscr_get_signalling_connection_status

DESCRIPTION
  This function gets signalling Connections to release status of Domain.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if there is an active Signalling connection. Otherwise it returns
  FALSE

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/

boolean tdsrrcscr_get_signalling_connection_status
(
  rrc_cn_domain_identity_e_type cn_domain_id                   
)
{
  boolean status = FALSE;      /* local variable for status */

  switch (cn_domain_id)
  {
    case RRC_CS_DOMAIN_CN_ID:
      if(tdsrrcdt_int_data.cs_domain_open)
      { 
        status = TRUE;
      }
      break;

    case RRC_PS_DOMAIN_CN_ID:
      if(tdsrrcdt_int_data.ps_domain_open)
      { 
        status = TRUE;
      }
      break;

    default:
      ERR_FATAL("Invalid CN domain: %d",cn_domain_id,0,0);

  }
  return(status);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

/*===========================================================================

FUNCTION   tdsrrcdt_is_dt_initiated_cu

DESCRIPTION
  This function checks if CU was initiated by IDT procedure
DEPENDENCIES
  None.

RETURN VALUE
  TRUE : If CU was initiated by IDT
  FALSE : Otherwise
===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/

boolean tdsrrcdt_is_dt_initiated_cu
(
  void
)
{
  boolean status = FALSE;
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();
  if(rrc_state == TDSRRC_STATE_CELL_PCH || rrc_state == TDSRRC_STATE_URA_PCH)
  {
    if(tdsrrcidt_cs_saved_open_session_req.cu_init_by_idt == TRUE ||
      tdsrrcidt_ps_saved_open_session_req.cu_init_by_idt == TRUE)
    {
      status = TRUE;
    }
  }
  return status;
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/


/*===========================================================================

FUNCTION RRCSCRR_PROCESS_SIG_CONN_REL_REQ

DESCRIPTION
  Check the current RRC state and send the sig conn req.If CU is active
  wait for CU to complete before sending the request

DEPENDENCIES
  None.

RETURN VALUE
  void

===========================================================================*/
void tdsrrcscrr_process_sig_conn_rel_req
(
  rrc_cn_domain_identity_e_type 
);

/*===========================================================================

FUNCTION tdsrrcscrr_send_close_session_cnf  

DESCRIPTION
  This function builds the RRC_CLOSE_SESSION_CNF command with parameters and 
  posts the same to the MM task.
DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcdt_status_e_type: It returns TDSRRCDT_SUCCESS if it is successfully sends 
  the RRC_CLOSE_SESSION_CNF command. Otherwise it returns TDSRRCDT_FAILURE_MAJOR.

===========================================================================*/

void tdsrrcscrr_send_close_session_cnf
(
  rrc_close_session_status_e_type     session_status,  /* Success or failure
                                                          status*/
  rrc_cn_domain_identity_e_type       cn_domain_id     /* CN Domain Id */

);

/* =======================================================================
**                            Function Definitions
** ======================================================================= */

/*===========================================================================

FUNCTION tdsrrcscr_release_signalling_connection

DESCRIPTION
  This function checks whether Signalling connection exist for the particular
  CN Domain and if it exits, then sends RRC_CLOSE_SESSION_IND/CNF to the MM. Also
  sets the local variable for signalling connection status to FALSE.


DEPENDENCIES
  None.

RETURN VALUE
  rrcscr_sig_conn_statue_e_type: it returns TDSRRCSCR_SIG_CONN_RELEASED if there
  exits a signalling connection for the CN domain specified in the formal 
  parameter and if RRCSCR procedure successfully sends RRC_CLOSE_SESSION_IND
  to the MM. Otherwise it retuns TDSRRCSCR_SIG_CONN_NOT_EXIST.

===========================================================================*/
tdsrrcscr_sig_conn_rel_status_e_type tdsrrcscr_local_release_of_signalling_connection
(
  rrc_cn_domain_identity_e_type cn_domain_id,
  rrc_session_close_cause_e_type cause
)
{
  tdsrrcscr_sig_conn_rel_status_e_type status = TDSRRCSCR_SIG_CONN_NOT_EXIST;
                                   /* Local parameter to store the status */
  
  switch(cn_domain_id)
  {
    case RRC_CS_DOMAIN_CN_ID:
       /* Check whether Signalling Connection for CS Domain is open */
      if(tdsrrcdt_int_data.cs_domain_open == TRUE)
      {
        if(TDSRRCDT_SUCCESS == tdsrrcscr_indicate_close_session(cn_domain_id, cause))
        {
          /* Set the Signalling connection status to closed */
          tdsrrcdt_int_data.cs_domain_open = FALSE;
          status = TDSRRCSCR_SIG_CONN_RELEASED;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"CS DOMAIN CLOSE WITH SUCCESS - DELETE ALL SAVED UDTs");
          tdsrrcudt_delete_all_saved_data_req(RRC_CS_DOMAIN_CN_ID);
          if(tdsrrcudt_any_saved_data_req_for_cn_domain(RRC_PS_DOMAIN_CN_ID) == FALSE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"There is no saved UDT for PS domain,set udt substate to init");
            tdsrrcudt_substate = TDSRRCUDT_INITIAL;
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to post RRC_CLOSE_SESSION_IND");
        }
      }

      break;

    case RRC_PS_DOMAIN_CN_ID:
      /* Check whether Signalling Connection for PS Domain is open */
      if(tdsrrcdt_int_data.ps_domain_open == TRUE)
      {
        if(TDSRRCDT_SUCCESS == tdsrrcscr_indicate_close_session(cn_domain_id, cause))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sent RRC_CLOSE_SESSION_IND");

          /* Set the Signalling connection status to closed */
          tdsrrcdt_int_data.ps_domain_open = FALSE;
          status = TDSRRCSCR_SIG_CONN_RELEASED;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"PS DOMAIN CLOSE WITH SUCCESS - DELETE ALL SAVED UDTs");
          tdsrrcudt_delete_all_saved_data_req(RRC_PS_DOMAIN_CN_ID);
          if(tdsrrcudt_any_saved_data_req_for_cn_domain(RRC_CS_DOMAIN_CN_ID) == FALSE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"There is no saved UDT for CS domain,set udt substate to init");
            tdsrrcudt_substate = TDSRRCUDT_INITIAL;
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to post RRC_CLOSE_SESSION_IND");
        }
      }
      break;

    default:
      ERR_FATAL("This should never happen",0,0,0);
      break;
  }
  return (status);
} /* tdsrrcscr_release_signalling_connection */
/*===========================================================================

FUNCTION tdsrrcscrr_is_close_session_ind_needed

DESCRIPTION
  This function will check & delete any pending CLOSE_SESSION_REQ for a  domain.
  based on scrr substae it will send either close_session_cnf/ind or sig_conn_rel 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: If close_session_ind needed for a particular CN domain.
  FALSE:Otherwise.
===========================================================================*/
static boolean tdsrrcscrr_is_close_session_ind_needed
(
  rrc_cn_domain_identity_e_type domain_id
)
{
  boolean status = TRUE;
  tdsrrcscrr_close_sess_req_type *save_close_session_req_ptr = NULL; /* Ptr to close session Req*/
  if(domain_id == RRC_CS_DOMAIN_CN_ID)
  {
    save_close_session_req_ptr = &tdsrrcscrr_saved_cs_close_session_req;
  }
  else
  {
    save_close_session_req_ptr = &tdsrrcscrr_saved_ps_close_session_req;
  }
  
  switch(domain_id)
  {
    case RRC_CS_DOMAIN_CN_ID :
      if(( save_close_session_req_ptr->close_sess_req_status == TRUE)|| 
          ( save_close_session_req_ptr->rrc_sig_con_rel_status == TRUE)||
          (save_close_session_req_ptr->mui_status == TDSRRCSCRR_MUI_CLOSE_SESSION_CNF)||
          (save_close_session_req_ptr->mui_status == TDSRRCSCRR_MUI_SIG_CON_REL_CNF))
      {
        if((save_close_session_req_ptr->mui_status == TDSRRCSCRR_MUI_CLOSE_SESSION_CNF)||
          ( save_close_session_req_ptr->close_sess_req_status == TRUE))
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"mui_status=%d,close_sess_req_status =%d",save_close_session_req_ptr->mui_status,save_close_session_req_ptr->close_sess_req_status);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Scrr need to send CLOSE_SESSION_CNF");
          tdsrrcscrr_send_close_session_cnf(RRC_CLOSE_SESS_SUCCESS,RRC_CS_DOMAIN_CN_ID);
          status = FALSE;
        }
        else if((save_close_session_req_ptr->mui_status == TDSRRCSCRR_MUI_SIG_CON_REL_CNF)||
           (save_close_session_req_ptr->rrc_sig_con_rel_status == TRUE))
        {
#if defined(FEATURE_3GPP_FAST_DORMANCY)
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"mui_status=%d, rrc_sig_con_rel_status =%d proc:%d",save_close_session_req_ptr->mui_status,
            save_close_session_req_ptr->rrc_sig_con_rel_status,
            save_close_session_req_ptr->rrc_proc);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Scrr need to send rrc_sig_con_rel_status");
          tdsrrcscrr_send_sig_conn_release_cnf(
                        SUCCESS, 
                        save_close_session_req_ptr->rrc_proc);
#else
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"mui_status=%d,rrc_sig_con_rel_status =%d",save_close_session_req_ptr->mui_status,save_close_session_req_ptr->rrc_sig_con_rel_status);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Scrr need to send rrc_sig_con_rel_status");
          tdsrrcscrr_send_sig_conn_release_cnf(SUCCESS);
#endif
          status = FALSE;
        }
        /*Its safe to call this function to delete tdsrrcscrr_saved_cs_close_session_req here with out 
        *any condition
        */
        tdsrrcscrr_delete_close_session_req(RRC_CS_DOMAIN_CN_ID);
        if(tdsrrcscrr_saved_ps_close_session_req.mui_status != TDSRRCSCRR_MUI_STATUS_FALSE)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Scrr sub state is %d,cont.. in this state as mui state of PS is %d ",tdsrrcscrr_substate,tdsrrcscrr_saved_ps_close_session_req.mui_status);
        }
        else if((tdsrrcscrr_saved_ps_close_session_req.close_sess_req_status == TRUE)||
          (tdsrrcscrr_saved_ps_close_session_req.rrc_sig_con_rel_status == TRUE))
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Scrr sub state is %d,cont.. in this state as close_req=%d sig_con_req=%d ",tdsrrcscrr_substate,
            tdsrrcscrr_saved_ps_close_session_req.close_sess_req_status,
            tdsrrcscrr_saved_ps_close_session_req.rrc_sig_con_rel_status);
        }
        else
        {
          tdsrrcscrr_substate = TDSRRCSCRR_INITIAL; 
        }
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Scrr sub state is %d,cn domain=%d",tdsrrcscrr_substate,domain_id);
      }
      else
      {
        /*Its safe to call this function to delete tdsrrcscrr_saved_cs_close_session_req here with out 
        *any condition
        */
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Scrr sub state is %d,cn domain=%d,PS mui_status=%d",
               tdsrrcscrr_substate,
               domain_id,
               tdsrrcscrr_saved_ps_close_session_req.mui_status);
        tdsrrcscrr_delete_close_session_req(RRC_CS_DOMAIN_CN_ID);
        if(tdsrrcscrr_saved_ps_close_session_req.mui_status != TDSRRCSCRR_MUI_STATUS_FALSE)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Scrr sub state is %d,cont.. in this state as mui state of PS is %d ",tdsrrcscrr_substate,tdsrrcscrr_saved_ps_close_session_req.mui_status);
        }
        else if((tdsrrcscrr_saved_ps_close_session_req.close_sess_req_status == TRUE)||
          (tdsrrcscrr_saved_ps_close_session_req.rrc_sig_con_rel_status == TRUE))
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Scrr sub state is %d,cont.. in this state as close_req=%d sig_con_req=%d ",tdsrrcscrr_substate,
            tdsrrcscrr_saved_ps_close_session_req.close_sess_req_status,
            tdsrrcscrr_saved_ps_close_session_req.rrc_sig_con_rel_status);
        }
        else
        {
          tdsrrcscrr_substate = TDSRRCSCRR_INITIAL; 
        }
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Scrr sub state is %d,cn domain=%d",tdsrrcscrr_substate,domain_id);
      }
    break;
  
    case RRC_PS_DOMAIN_CN_ID :
      if(( save_close_session_req_ptr->close_sess_req_status == TRUE)|| 
          ( save_close_session_req_ptr->rrc_sig_con_rel_status == TRUE)||
          (save_close_session_req_ptr->mui_status == TDSRRCSCRR_MUI_CLOSE_SESSION_CNF)||
          (save_close_session_req_ptr->mui_status == TDSRRCSCRR_MUI_SIG_CON_REL_CNF))
      {
        if((save_close_session_req_ptr->mui_status == TDSRRCSCRR_MUI_CLOSE_SESSION_CNF)||
          ( save_close_session_req_ptr->close_sess_req_status == TRUE))
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"mui_status=%d,close_sess_req_status =%d",save_close_session_req_ptr->mui_status,save_close_session_req_ptr->close_sess_req_status);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Scrr need to send CLOSE_SESSION_CNF");
          tdsrrcscrr_send_close_session_cnf(RRC_CLOSE_SESS_SUCCESS,RRC_PS_DOMAIN_CN_ID);
          status = FALSE;
        }
        else if((save_close_session_req_ptr->mui_status == TDSRRCSCRR_MUI_SIG_CON_REL_CNF)||
           (save_close_session_req_ptr->rrc_sig_con_rel_status == TRUE))
        {
#if defined(FEATURE_3GPP_FAST_DORMANCY)
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"mui_status=%d, rrc_sig_con_rel_status =%d proc:%d",save_close_session_req_ptr->mui_status,
            save_close_session_req_ptr->rrc_sig_con_rel_status,
            save_close_session_req_ptr->rrc_proc);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Scrr need to send rrc_sig_con_rel_status");
          tdsrrcscrr_send_sig_conn_release_cnf(
                        SUCCESS,
                        save_close_session_req_ptr->rrc_proc);
#else
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"mui_status=%d,rrc_sig_con_rel_status =%d",save_close_session_req_ptr->mui_status,save_close_session_req_ptr->rrc_sig_con_rel_status);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Scrr need to send rrc_sig_con_rel_status");
          tdsrrcscrr_send_sig_conn_release_cnf(SUCCESS);
#endif
          status = FALSE;
        }
        /*Its safe to call this function to delete tdsrrcscrr_saved_cs_close_session_req here with out 
        *any condition
        */
        tdsrrcscrr_delete_close_session_req(RRC_PS_DOMAIN_CN_ID);
        if(tdsrrcscrr_saved_cs_close_session_req.mui_status != TDSRRCSCRR_MUI_STATUS_FALSE)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Scrr sub state is %d,cont.. in this state as mui state of CS is %d ",tdsrrcscrr_substate,tdsrrcscrr_saved_cs_close_session_req.mui_status);
        }
        else if((tdsrrcscrr_saved_cs_close_session_req.close_sess_req_status == TRUE)||
          (tdsrrcscrr_saved_cs_close_session_req.rrc_sig_con_rel_status == TRUE))
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Scrr sub state is %d,cont.. in this state as close_req=%d sig_con_req=%d ",tdsrrcscrr_substate,
            tdsrrcscrr_saved_cs_close_session_req.close_sess_req_status,
            tdsrrcscrr_saved_cs_close_session_req.rrc_sig_con_rel_status);
        }
        else
        {
          tdsrrcscrr_substate = TDSRRCSCRR_INITIAL; 
        }
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Scrr sub state is %d,cn domain=%d",tdsrrcscrr_substate,domain_id);
      }
      else
      {
        /*Its safe to call this function to delete tdsrrcscrr_saved_cs_close_session_req here with out 
        *any condition
        */
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Scrr sub state is %d,cn domain=%d,CS mui_status=%d",
             tdsrrcscrr_substate,
             domain_id,
             tdsrrcscrr_saved_cs_close_session_req.mui_status);
        tdsrrcscrr_delete_close_session_req(RRC_PS_DOMAIN_CN_ID);
        if(tdsrrcscrr_saved_cs_close_session_req.mui_status != TDSRRCSCRR_MUI_STATUS_FALSE)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Scrr sub state is %d,cont.. in this state as mui state of CS is %d ",tdsrrcscrr_substate,tdsrrcscrr_saved_cs_close_session_req.mui_status);
        }
        else if((tdsrrcscrr_saved_cs_close_session_req.close_sess_req_status == TRUE)||
          (tdsrrcscrr_saved_cs_close_session_req.rrc_sig_con_rel_status == TRUE))
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Scrr sub state is %d,cont.. in this state as close_req=%d sig_con_req=%d ",tdsrrcscrr_substate,
            tdsrrcscrr_saved_cs_close_session_req.close_sess_req_status,
            tdsrrcscrr_saved_cs_close_session_req.rrc_sig_con_rel_status);
        }
        else
        {
          tdsrrcscrr_substate = TDSRRCSCRR_INITIAL; 
        }
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Scrr sub state is %d,cn domain=%d",tdsrrcscrr_substate,domain_id);
      }
    break;
  
    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Incorrect CN Domain I.D=%d",domain_id);
    break;
  }
  return(status);
}

/*===========================================================================

FUNCTION tdsrrcscr_indicate_close_session

DESCRIPTION
  This function will check & delete any pending CLOSE_SESSION_REQ for a  domain.
  based on scrr substae it will send either close_session_cnf/ind or sig_conn_rel 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: If IDT is buffered for a particular CN domain.
  FALSE:Otherwise.
===========================================================================*/
tdsrrcdt_status_e_type tdsrrcscr_indicate_close_session
(
  rrc_cn_domain_identity_e_type       cn_domain_id,     /* CN Domain Id */
  rrc_session_close_cause_e_type      cause
)
{
  tdsrrcdt_status_e_type status = TDSRRCDT_SUCCESS;
  if(tdsrrcscrr_is_close_session_ind_needed(cn_domain_id) == TRUE)
  {
    status = tdsrrcscr_send_close_session_ind(cn_domain_id, cause);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sent RRC_CLOSE_SESSION_IND");

#if defined(FEATURE_3GPP_FAST_DORMANCY)
    if(RRC_PS_DOMAIN_CN_ID == cn_domain_id)
    {
      /* Abort Dormancy if SCRI for CLOSE_SESSION_REQ is sended */
      tdsrrcdormancy_check_and_abort_dormancy_req(RRC_FAST_DORMANCY_SUCCESS_APP_NO_RETRY);
    }
#endif
  }
  
  return(status);
}
/*===========================================================================

FUNCTION tdsrrcidt_is_idt_buffered

DESCRIPTION
  This function will check & return the status of 
  IDT is being buffered for the domain.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: If IDT is buffered for a particular CN domain.
  FALSE:Otherwise.
===========================================================================*/
static boolean tdsrrcidt_is_idt_buffered(rrc_cn_domain_identity_e_type domain_id)
{
  boolean status = FALSE;

    switch(domain_id)
    {
      case RRC_CS_DOMAIN_CN_ID :
      if(tdsrrcidt_cs_saved_open_session_req.open_sess_req_status == TRUE)
      {
        status = TRUE;
      }
      break;

      case RRC_PS_DOMAIN_CN_ID :
      if(tdsrrcidt_ps_saved_open_session_req.open_sess_req_status == TRUE)
      {
        status = TRUE;
      }
      break;

     default:
       TDSRRC_MSG1(MSG_LEGACY_HIGH,"Incorrect CN Domain I.D=%d",domain_id);
       break;
    }
return status;
}
/*===========================================================================

FUNCTION RRCDT_PURGE_RLC_WM_RB3_RB4

DESCRIPTION

  This function requests RLC to purge RLC UL  watermark for RB3 and RB4
  
DEPENDENCIES


RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
static void tdsrrcdt_purge_rlc_wm_rb3_rb4
(    
  boolean purge_srb3, boolean purge_srb4, tdsrrc_proc_e_type proc_id  
)
{
  uint8 count = 0;            /* Count of LC Ids to be purged */
  tdsrrc_cmd_type *llc_cmd_ptr;  /* Pointer to the RRC Command */

  /* Send CRLC DATA IND REQ to RLC via rrcllc */
  if((llc_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    tdsl2_ul_req_cmd * llc_cmd_l2_ptr = NULL;
    tdsrrc_get_pointer_l2_req_value(&llc_cmd_ptr->cmd.rrc_llc_req.l2_ul_req, &llc_cmd_l2_ptr);

    /* Initialize the LLC command header first */
    llc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_LLC_CMD_REQ;
    llc_cmd_ptr->cmd.rrc_llc_req.procedure = proc_id;
    llc_cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER2_RLC_UL;

    /* Fill the L2 cmd header */
    llc_cmd_ptr->cmd.rrc_llc_req.l2_ul_cmd.cmd_id = TDSRLC_UL_PURGE_WM_REQ;

    /* Include the lc_ids for signalling RBs with RLC AM mode. */
    if(purge_srb3)
    {
      llc_cmd_l2_ptr->ul_purge_wm_ind.lc_id[count] =
        tdsrrclcm_get_ul_rlc_lc_id(DCCH_DT_HIGH_PRI_RADIO_BEARER_ID);
      llc_cmd_l2_ptr->ul_purge_wm_ind.num_sdus[count++] = 
        TDSMAX_SRB2_SDUS_TOBE_PURGED;
    }
    if(purge_srb4)
    {
      llc_cmd_l2_ptr->ul_purge_wm_ind.lc_id[count] =
        tdsrrclcm_get_ul_rlc_lc_id(DCCH_DT_LOW_PRI_RADIO_BEARER_ID);
      llc_cmd_l2_ptr->ul_purge_wm_ind.num_sdus[count++] = 
        TDSMAX_SRB2_SDUS_TOBE_PURGED;
    }

    /* Populate the total number of LCs to be purged */
    llc_cmd_l2_ptr->ul_purge_wm_ind.num_lc_id = count;
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Purging RLC SRB3:%d, SRB4:%d, proc_id:%d",purge_srb3,purge_srb4,proc_id);
    tdsrrc_put_int_cmd(llc_cmd_ptr); 
  }
  else
  {
    ERR_FATAL("RRC Unable to get cmd buf for RLC cmd",0,0,0);
  }
}

/*===========================================================================

FUNCTION RRCDT_CHECK_AND_SUSPEND_BPLMN

DESCRIPTION
  This functions checks if BPLMN is in progress and if it is,then sends 
  a Suspend Req to CSP with the establishment cause

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcdt_check_and_suspend_bplmn
(
  rrc_establish_cause_e_type  est_cause
)
{
  tdsrrc_bplmn_suspend_cause_e_type suspend_cause;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Checking if BPLMN is active when TDSRRC_OPEN_SESSION_REQ is received");

  if(TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"BPLMN active when TDSRRC_OPEN_SESSION_REQ is received");
     /* Map the establishment cause to the appropriate suspend cause */
    switch(est_cause)
    {
       case RRC_EST_REGISTRATION:
         suspend_cause = TDSBPLMN_SUSPEND_REASON_REGISTRATION;
         break;
       default:
         suspend_cause = TDSBPLMN_SUSPEND_REASON_OTHER;
         break;
    }
    /* Send the bplmn suspend request */
    tdsrrccsp_send_bplmn_suspend_req(TDSRRC_PROCEDURE_IDT,suspend_cause);
  }
}

/*===========================================================================

FUNCTION RRCDT_CHECK_AND_SEND_PENDING_SCRR

DESCRIPTION
  Processes pending SCRR if any after IDT has been completed

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If SCRR for the domain was buffered and is being sent out now
  FALSE - SCRR for the domain is not active

===========================================================================*/
static boolean tdsrrcdt_check_and_send_pending_scrr
(
  rrc_cn_domain_identity_e_type domain_id  
)
{
  /* Boolean to indicate that there was a SCRR pending and it is being sent out 
     now.So if IDT had received a NACK,it need not pursue the re-transmissions since 
	 the signalling connection is being released */
  boolean ret_val = FALSE;

  /* Check to see if there is a SCRR yet to be processed for this domain.Make sure to
     check the mui status also because retranmisssions are not handled in this function.
	 This is only for sending the SCRR out for the first time because it was delayed
	 due to IDT being active at that time */
  switch(domain_id)
  {
    case RRC_CS_DOMAIN_CN_ID:
      if(((tdsrrcscrr_saved_cs_close_session_req.close_sess_req_status == TRUE)||
          (tdsrrcscrr_saved_cs_close_session_req.rrc_sig_con_rel_status == TRUE)) &&
         (tdsrrcscrr_saved_cs_close_session_req.mui_status == TDSRRCSCRR_MUI_STATUS_FALSE))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing saved SCRR for CS domain after IDT indication");
        tdsrrcscrr_process_sig_conn_rel_req(domain_id);
        ret_val = TRUE;
      }
      break;

    case RRC_PS_DOMAIN_CN_ID:
      if(((tdsrrcscrr_saved_ps_close_session_req.close_sess_req_status == TRUE)||
          (tdsrrcscrr_saved_ps_close_session_req.rrc_sig_con_rel_status == TRUE)) &&
         (tdsrrcscrr_saved_ps_close_session_req.mui_status == TDSRRCSCRR_MUI_STATUS_FALSE))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing saved SCRR for PS domain after IDT indication");
        tdsrrcscrr_process_sig_conn_rel_req(domain_id);
        ret_val = TRUE;
      }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid domain id %d",domain_id);
      break;
  }
  return ret_val;
}

/*===========================================================================

FUNCTION tdsrrcidt_check_if_est_cause_available  

DESCRIPTION
  This function gets the establishment cause value to be filled in OTA message.
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE indicates valid value is present.

===========================================================================*/
boolean tdsrrcidt_check_if_est_cause_available
(
       tdsrrc_proc_e_type proc_id,
       rrc_establish_cause_e_type  *est_cause
       ,rrc_call_type_in_est_req_e_type *call_type
)
{
  boolean ret_val =FALSE;

  if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)) 
  {
    if((tdsrrcidt_est_cause_info.valid == TRUE) && (tdsrrcidt_est_cause_info.to_be_sent_by == proc_id))
    {
      ret_val = TRUE;
      *est_cause = tdsrrcidt_est_cause_info.est_cause;
      *call_type = tdsrrcidt_est_cause_info.call_type;
    }
  }
  return ret_val;
}

/*===========================================================================

FUNCTION tdsrrcidt_check_if_est_cause_available  

DESCRIPTION
  This function gets the establishment cause value to be filled in OTA message.
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE indicates valid value is present.

===========================================================================*/
boolean tdsrrcidt_is_cs_call_initiate
(
  void
)
{
  boolean ret_val =FALSE;
  if(tdsrrcidt_cs_saved_open_session_req.cu_init_by_idt ==TRUE)
  {
    ret_val = TRUE;
  }
  return ret_val;
}


/*===========================================================================

FUNCTION tdsrrcidt_change_owner_est_cause  

DESCRIPTION
  This function updates the to_be_sent flag with procedure id
  
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcidt_change_owner_est_cause
(
    tdsrrc_proc_e_type proc_id 
)
{
  if(proc_id == TDSRRC_PROCEDURE_NONE)
  {
    return;
  }
  if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)) 
  {
    if(tdsrrcidt_est_cause_info.valid == TRUE)
    {
      tdsrrcidt_est_cause_info.to_be_sent_by=proc_id;
    }
  }
}

/*===========================================================================

FUNCTION tdsrrcidt_fill_est_cause  

DESCRIPTION
  This function fills the establishment cause in ASN structure.
  
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcidt_fill_est_cause
(
  tdsrrc_UL_DCCH_Message     *msg_ptr         /* Pointer to uplink DCCH message msg_ptr*/,
  rrc_cn_domain_identity_e_type       dom_id   
)
{
  rrc_establish_cause_e_type  est_cause;
  tdsrrc_EstablishmentCause  trans_cause;
  rrc_call_type_in_est_req_e_type call_type;
  if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)) 
  {

    if(tdsrrcidt_check_if_est_cause_available(TDSRRC_PROCEDURE_IDT, &est_cause
                                                                    ,&call_type
                                                                    )== TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Filling the establishment cause ");
      /* Enable the v3a0NonCriticalExtension structure */
  
     /* Enable the laterNonCriticalExtension field in v3a0NoncriticalExtension structure*/
      TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions,
                                rrc_InitialDirectTransfer_v3a0NonCriticalExtensions_,
                                laterNonCriticalExtensions);
  
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
        laterNonCriticalExtensions);
  
      /* Enable the v590NonCriticalExtension field in laterNonCriticl in v3a0Non critical */
      TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
                                laterNonCriticalExtensions,
                                rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions_,
                                v590NonCriticalExtensions);
  
      /* set bit_mask to 0 to indicate nonCriticalExtensions to v590 extensions are not present */
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
               v590NonCriticalExtensions);
  
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
               v590NonCriticalExtensions.initialDirectTransfer_v590ext);
  
      /* Enable the establishment cause */
      TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
                          laterNonCriticalExtensions.v590NonCriticalExtensions.initialDirectTransfer_v590ext,
                          rrc_,
                          establishmentCause);
      if(dom_id == RRC_CS_DOMAIN_CN_ID)
      {
        est_cause = tdsrrcidt_cs_saved_open_session_req.est_cause;
      }
      else
      {
        est_cause = tdsrrcidt_ps_saved_open_session_req.est_cause;
      }
  
      tdsrrc_translate_establishment_cause(est_cause, &trans_cause);
  
      /* Fill the establishment cause */
      msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
               v590NonCriticalExtensions.initialDirectTransfer_v590ext.establishmentCause = trans_cause;            

      if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
      {
        if((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && 
          (TDSOC_SET_FOR_CELL_DCH != tdsrrcllc_get_ordered_config_status())&&
          (dom_id ==RRC_CS_DOMAIN_CN_ID))
        {
          if(est_cause == RRC_EST_ORIG_CONV_CALL ||
             est_cause == RRC_EST_EMERGENCY_CALL)
          {
            if(!(TDSRRCRBR_R7_MSG_IE_PRESENT(&msg_ptr->message.u.initialDirectTransfer,
                    tdsrrc_InitialDirectTransfer,
                    v3a0NonCriticalExtensions)))
          {
              TDSRRCSET_R7_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer,
                    rrc_InitialDirectTransfer_,
                    v3a0NonCriticalExtensions);
              TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions);
              TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.initialDirectTransfer_v3a0ext);
          }
            if(!(TDSRRCRBR_R7_MSG_IE_PRESENT(&msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions,
                     tdsrrc_InitialDirectTransfer_v3a0NonCriticalExtensions,
                     laterNonCriticalExtensions)))
          {
              TDSRRCSET_R7_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions,
                     rrc_InitialDirectTransfer_v3a0NonCriticalExtensions_,
                     laterNonCriticalExtensions);
              TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions);
          }
            if(!(TDSRRCRBR_R7_MSG_IE_PRESENT(&msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
                     laterNonCriticalExtensions,
                     rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions,
                     v590NonCriticalExtensions)))
          {
              TDSRRCSET_R7_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
                     laterNonCriticalExtensions,
                     rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions_,
                     v590NonCriticalExtensions);
              TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
                v590NonCriticalExtensions);
              TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
                v590NonCriticalExtensions.initialDirectTransfer_v590ext);
          }
            if(!(TDSRRCRBR_R7_MSG_IE_PRESENT(&msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
                     laterNonCriticalExtensions.v590NonCriticalExtensions,
                     rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions_v590NonCriticalExtensions,
                     v690NonCriticalExtensions)))
          {
              TDSRRCSET_R7_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
                     laterNonCriticalExtensions.v590NonCriticalExtensions,
                     rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions_v590NonCriticalExtensions_,
                     v690NonCriticalExtensions);
              TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
                v590NonCriticalExtensions.v690NonCriticalExtensions);
              TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
                v590NonCriticalExtensions.v690NonCriticalExtensions.initialDirectTransfer_v690ext);
          }
            if(!(TDSRRCRBR_R7_MSG_IE_PRESENT(&msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
                     laterNonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions,
                     rrc_v590NonCriticalExtensions_v690NonCriticalExtensions,
                     v770NonCriticalExtensions)))
          {
              TDSRRCSET_R7_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
                     laterNonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions,
                     rrc_v590NonCriticalExtensions_v690NonCriticalExtensions_,
                     v770NonCriticalExtensions);
              TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
                v590NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions);
              TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
                v590NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.initialDirectTransfer_v770ext);
          }
            TDSRRCSET_R7_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
                    laterNonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
                    v770NonCriticalExtensions.initialDirectTransfer_v770ext,
                    rrc_InitialDirectTransfer_v770ext_IEs_,
                    csCallType);
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"Establishment cause : %d Call Type : %d " ,est_cause,call_type);
            
            switch(call_type)
            {
              case RRC_CALL_TYPE_SPEECH:
                msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
                           v590NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.initialDirectTransfer_v770ext.
                                                              csCallType = tdsrrc_InitialDirectTransfer_v770ext_IEs_csCallType_speech;
                break;
              case RRC_CALL_TYPE_VIDEO:
                msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
                           v590NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.initialDirectTransfer_v770ext.
                                                              csCallType = tdsrrc_InitialDirectTransfer_v770ext_IEs_csCallType_video;
                break;
              case RRC_CALL_TYPE_OTHERS:
                msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
                           v590NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.initialDirectTransfer_v770ext.
                                                              csCallType = tdsrrc_InitialDirectTransfer_v770ext_IEs_csCallType_other;
                break;
              default:
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"Wrong est call type set by NAS = %d",call_type);
                msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
                           v590NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.initialDirectTransfer_v770ext.
                                                              csCallType = tdsrrc_InitialDirectTransfer_v770ext_IEs_csCallType_other;
                break;
            }
          }
        }
      }

    }
  }
}


/*===========================================================================

FUNCTION tdsrrcidt_set_sent_by_cu  

DESCRIPTION
  This function sets the variable to indicate the transmission was done by CU
  
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcidt_set_sent_by_cu
(   
  tdsrrc_proc_e_type proc_id
)
{

  if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)) 
  {
    rrc_establish_cause_e_type  est_cause;

    rrc_call_type_in_est_req_e_type call_type;

    if(tdsrrcidt_check_if_est_cause_available(proc_id,&est_cause
                                                                    ,&call_type
                                                                   )==TRUE)
    {
      tdsrrcidt_est_cause_info.sent_by_cu= TRUE;
    }
  }
}

/*===========================================================================

FUNCTION tdsrrcidt_set_est_cause  

DESCRIPTION
  This function updates the establishment cause to be used later while transmitting.
  
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcidt_set_est_cause
(  
   rrc_establish_cause_e_type  est_cause, 
   tdsrrc_proc_e_type proc_id,
   rrc_cn_domain_identity_e_type       cn_domain_id     /* CN Domain Id */
)
{
  if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)) 
  {
    boolean multi_rab=FALSE;
    if(tdsrrcidt_est_cause_info.last_proc_sent == TDSRRC_PROCEDURE_RCE)
    {/*First IDT will not carry EST_cause*/
      tdsrrcidt_est_cause_info.est_cause = est_cause;
      tdsrrcidt_est_cause_info.last_proc_sent = proc_id;
      tdsrrcidt_est_cause_info.valid = FALSE;
    }
    else
    {  /*All subsequent IDT's will carry EST_cause which 
         uses same rrc connection*/
      tdsrrcidt_est_cause_info.last_proc_sent = proc_id;

      tdsrrcidt_est_cause_info.valid = TRUE;
      tdsrrcidt_est_cause_info.est_cause=est_cause;
      tdsrrcidt_est_cause_info.to_be_sent_by = proc_id;

      if(cn_domain_id == RRC_CS_DOMAIN_CN_ID)
      {
        tdsrrcidt_est_cause_info.call_type = tdsrrcidt_cs_saved_open_session_req.open_session_req.call_type;
      }
      //multi_rab is retained as such though no use since its used below for debugging 
      // purpose. If its not needed this can be removed
      multi_rab = TRUE; 
    }
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"last_proc_sent = %d, est_cause=%d & is valid =%d ",tdsrrcidt_est_cause_info.last_proc_sent,
                                                                                                    tdsrrcidt_est_cause_info.est_cause,
                                                                                                    tdsrrcidt_est_cause_info.valid);
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"multi_rab status =%d,prcd to be sent by =%d",multi_rab,tdsrrcidt_est_cause_info.to_be_sent_by);
  }
}
/*===========================================================================

FUNCTION tdsrrcidt_update_proc_id  

DESCRIPTION
  This function will be called by RCE to update the variable 
  last_proc_sent so that IDT cause can be skipped from filling
  for the first time

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcidt_update_proc_id()
{
  tdsrrcidt_est_cause_info.last_proc_sent = TDSRRC_PROCEDURE_RCE;
}
/*===========================================================================

FUNCTION tdsrrcidt_check_if_transmission_success  

DESCRIPTION
  This function checks if CU was successful in transmitting it over the air
  This can happen if priority of cell update cause calculation.
  
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcidt_check_if_transmission_success
(  
   tdsrrc_proc_e_type proc_id
)
{
  if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)) 
  {
    rrc_establish_cause_e_type  est_cause;

    rrc_call_type_in_est_req_e_type call_type;

    if (tdsrrcidt_check_if_est_cause_available(proc_id,&est_cause
                                                                    ,&call_type
                                                                    )==TRUE)
    {
      if(tdsrrcidt_est_cause_info.sent_by_cu==FALSE)
      {
        tdsrrcidt_est_cause_info.to_be_sent_by =TDSRRC_PROCEDURE_IDT;
      }
    }
  }
}

/*===========================================================================

FUNCTION tdsrrcidt_reset_est_cause  

DESCRIPTION
  This function resets the establishment cause once the transmission is successful.
  
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcidt_reset_est_cause
(  
   tdsrrc_proc_e_type proc_id)
{

  if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)) 
  {
    if((tdsrrcidt_est_cause_info.valid == TRUE) && 
       (tdsrrcidt_est_cause_info.to_be_sent_by == proc_id))
    {
      tdsrrcidt_est_cause_info.valid = FALSE;
      tdsrrcidt_est_cause_info.est_cause=RRC_EST_TERM_UNKNOWN;
      tdsrrcidt_est_cause_info.to_be_sent_by = TDSRRC_PROCEDURE_NONE;
      tdsrrcidt_est_cause_info.sent_by_cu = FALSE;

      tdsrrcidt_est_cause_info.call_type = RRC_CALL_TYPE_NONE;
    }
  }
}


/*===========================================================================

FUNCTION tdsrrcidt_delete_open_session_req

DESCRIPTION
  This function deletes the open session request message contents which
  were allocated memory by CELL_FACH, URA_PCH/CELL_PCH states
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
static void tdsrrcidt_delete_open_session_req
(
  rrc_cn_domain_identity_e_type       cn_domain_id     /* CN Domain Id */
)
{
  if(cn_domain_id == RRC_CS_DOMAIN_CN_ID)
  {
    tdsrrcidt_cs_saved_open_session_req.open_sess_req_status = FALSE;
    tdsrrcidt_cs_saved_open_session_req.mui_status = FALSE;
    tdsrrcidt_cs_saved_open_session_req.open_sess_cnf_required = FALSE;
    tdsrrcidt_cs_saved_open_session_req.cu_init_by_idt = FALSE;
    tdsrrcidt_cs_saved_open_session_req.fresh_start_value_needed = FALSE;

    /*delete heap memory if it were allocated*/
    if(tdsrrcidt_cs_saved_open_session_req.open_session_req.nas_msg.value != NULL)
    {
      tdsrrc_free( tdsrrcidt_cs_saved_open_session_req.open_session_req.nas_msg.value);

      tdsrrcidt_cs_saved_open_session_req.open_session_req.nas_msg.value = NULL;
      tdsrrcidt_cs_saved_open_session_req.open_session_req.nas_msg.length = 0;
    }
    tdsrrcidt_cs_saved_open_session_req.est_cause = RRC_EST_TERM_UNKNOWN;
    tdsrrcidt_reset_est_cause(TDSRRC_PROCEDURE_IDT);

    tdsrrcidt_cs_saved_open_session_req.open_session_req.call_type = RRC_CALL_TYPE_NONE;
  }
  else if(cn_domain_id == RRC_PS_DOMAIN_CN_ID)
  {
    tdsrrcidt_ps_saved_open_session_req.open_sess_req_status = FALSE;
    tdsrrcidt_ps_saved_open_session_req.mui_status = FALSE;
    tdsrrcidt_ps_saved_open_session_req.open_sess_cnf_required = FALSE;
    tdsrrcidt_ps_saved_open_session_req.cu_init_by_idt = FALSE;
    tdsrrcidt_ps_saved_open_session_req.fresh_start_value_needed = FALSE;

    /*delete heap memory if it were allocated*/
    if(tdsrrcidt_ps_saved_open_session_req.open_session_req.nas_msg.value != NULL)
    {
      tdsrrc_free( tdsrrcidt_ps_saved_open_session_req.open_session_req.nas_msg.value);

      tdsrrcidt_ps_saved_open_session_req.open_session_req.nas_msg.value = NULL;
      tdsrrcidt_ps_saved_open_session_req.open_session_req.nas_msg.length = 0;

    }
    tdsrrcidt_ps_saved_open_session_req.est_cause=RRC_EST_TERM_UNKNOWN;
    tdsrrcidt_reset_est_cause(TDSRRC_PROCEDURE_IDT);
  }
  else
  {
    ERR_FATAL("Invalid Domain ID %d",cn_domain_id,0,0);
  }

  /* Check and send any blocked SCRR */
  (void)tdsrrcdt_check_and_send_pending_scrr(cn_domain_id);

}/*tdsrrcidt_delete_open_session_req*/
/*===========================================================================

FUNCTION   tdsrrcidt_is_idt_active

DESCRIPTION
  This function checks if idt for the given domain is in progress
DEPENDENCIES
  None.

RETURN VALUE
  TRUE indicates that it is active, FALSE otherwise.

===========================================================================*/

boolean tdsrrcidt_is_idt_active
(
   rrc_cn_domain_identity_e_type domain_id
)
{
  boolean ret_val = FALSE;
  switch(domain_id)
  {
    case RRC_CS_DOMAIN_CN_ID:
      /* first case says that open session is received, eg waiting for CU */
      /* second condition says that we are waiting for l2ack */
      if((tdsrrcidt_cs_saved_open_session_req.open_sess_req_status == TRUE)||
         (tdsrrcidt_cs_saved_open_session_req.open_sess_cnf_required == TRUE))
      {
        ret_val = TRUE;
      }
      break;

    case RRC_PS_DOMAIN_CN_ID:
      /* first case says that open session is received, eg waiting for CU */
      /* second condition says that we are waiting for l2ack */
      if((tdsrrcidt_ps_saved_open_session_req.open_sess_req_status == TRUE)||
         (tdsrrcidt_ps_saved_open_session_req.open_sess_cnf_required == TRUE))
      {
        ret_val = TRUE;
      }
      break;

    default:
          
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid domain id");
      break;
  }
  return (ret_val);
}
/*===========================================================================

FUNCTION tdsrrcidt_can_buffered_idt_be_deleted

DESCRIPTION
  This function will check whether IDT has not initiated CU
  (a case in cell/URA  PCH),If yes then it shall delete IDT, else
  this funtion does nothing.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE:If IDt is deleted.
  FALSE:Otherwise.

===========================================================================*/
static boolean tdsrrcidt_can_buffered_idt_be_deleted
(
  rrc_cn_domain_identity_e_type domain_id
)
{
  boolean status=FALSE;
  switch(domain_id)
  {
    case RRC_CS_DOMAIN_CN_ID:
      if(tdsrrcidt_cs_saved_open_session_req.cu_init_by_idt == FALSE)
      {
        tdsrrcidt_delete_open_session_req(RRC_CS_DOMAIN_CN_ID);
        status = TRUE;
        if(FALSE == tdsrrcidt_is_idt_active(RRC_PS_DOMAIN_CN_ID))
        {
          tdsrrcidt_substate = TDSRRCIDT_INITIAL;  
        }
      }
      break;

    case RRC_PS_DOMAIN_CN_ID:
      if(tdsrrcidt_ps_saved_open_session_req.cu_init_by_idt == FALSE)
      {
        tdsrrcidt_delete_open_session_req(RRC_PS_DOMAIN_CN_ID);
        status = TRUE;
        if(FALSE == tdsrrcidt_is_idt_active(RRC_CS_DOMAIN_CN_ID))
        {
          tdsrrcidt_substate = TDSRRCIDT_INITIAL;  
        }
      }
      break;

    default:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid domain id");
      break;
  }
  return status;
}
/*===========================================================================

FUNCTION   tdsrrcidt_clear_idt_data_for_race_conditions

DESCRIPTION

RETURN VALUE
  

===========================================================================*/
static void tdsrrcidt_clear_idt_data_for_race_conditions
(
   rrc_cn_domain_identity_e_type domain_id
)
{
  /* wait for IDT to complete if it is still in progress*/
  if( tdsrrcidt_is_idt_active(domain_id) == TRUE)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"DELETE IDT active for domain %d",domain_id);

    switch(domain_id)
    {
      case RRC_CS_DOMAIN_CN_ID:
        tdsrrcidt_delete_open_session_req(RRC_CS_DOMAIN_CN_ID);
        if(FALSE == tdsrrcidt_is_idt_active(RRC_PS_DOMAIN_CN_ID))
        {
          tdsrrcidt_substate = TDSRRCIDT_INITIAL;  
        }
        break;

      case RRC_PS_DOMAIN_CN_ID:
        tdsrrcidt_delete_open_session_req(RRC_PS_DOMAIN_CN_ID);
        if(FALSE == tdsrrcidt_is_idt_active(RRC_CS_DOMAIN_CN_ID))
        {
          tdsrrcidt_substate = TDSRRCIDT_INITIAL;  
        }
        break;

      default:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid domain id");
        break;
    }
  }
}
/*===========================================================================

FUNCTION tdsrrcidt_send_open_session_cnf  

DESCRIPTION
  This function builds the RRC_OPEN_SESSION_CNF command with parameters and 
  posts the same to the MM task.
DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcdt_status_e_type: It returns TDSRRCDT_SUCCESS if it is successfully sends 
  the RRC_OPEN_SESSION_CNF command. Otherwise it returns TDSRRCDT_FAILURE_MAJOR.

===========================================================================*/

void tdsrrcidt_send_open_session_cnf
(
  rrc_open_session_status_e_type      session_status,  /* Success or failure
                                                          status*/
  rrc_cn_domain_identity_e_type       cn_domain_id     /* CN Domain Id */

)
{
  mm_cmd_type *cmd_ptr;                      /* Pointer to the MM Command */

  /*Reset the last act rsp cause here  not at tdsrrcmcm_set_establishment_cause . Because the RRC Connection req
    may fail then retry , if NAS retry, the establish cause in RRC Connection req should be also 
    related with act rsp.*/
  tdsrrcmcm_reset_last_act_cause();
  /* Allocate memory for RRC_OPEN_SESSION_CNF command */
  if( (cmd_ptr =  mm_tdsrrc_get_cmd_buf(RRC_OPEN_SESSION_CNF)) != NULL)
  {
    /* Fill in parameters fro RRC_OPEN_SESSION_CNF command */
    cmd_ptr->cmd.rrc_open_session_cnf.cn_domain_id= cn_domain_id ;

    cmd_ptr->cmd.rrc_open_session_cnf.status = session_status;

#ifdef FEATURE_DUAL_SIM
    cmd_ptr->cmd.rrc_open_session_cnf.as_id = tdsrrc_get_as_id();
#endif

    /* Sends the command to MM */
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(cmd_ptr);
    }
    mm_put_cmd(cmd_ptr);

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sent RRC_OPEN_SESS_CNF to MM");

  }    
  else   /* RRC Task failed to allocate memory */
  {
    ERR_FATAL("Unable to allocate memory for MM Cmd: %d",RRC_OPEN_SESSION_CNF,0,0);
  }     

} /* tdsrrcidt_send_open_session_cnf */

/*===========================================================================

FUNCTION tdsrrcidt_send_sync_ind  

DESCRIPTION
  This function builds the RRC_SYND_IND command with parameters and 
  posts the same to the MM task.
DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcdt_status_e_type: It returns TDSRRCDT_SUCCESS if it is successfully sends 
  the RRC_SYNC_IND command. Otherwise it returns TDSRRCDT_FAILURE_MAJOR.

===========================================================================*/

static void tdsrrcidt_send_sync_ind
(
  rrc_sync_cause_e_type               cause_status,   /*  Cause Status */
  rrc_cn_domain_identity_e_type       cn_domain_id     /* CN Domain Id */

)
{
  mm_cmd_type *cmd_ptr;                      /* Pointer to the MM Command */

  /* Allocate memory for RRC_OPEN_SESSION_CNF command */
  if( (cmd_ptr =  mm_tdsrrc_get_cmd_buf(RRC_SYNC_IND)) != NULL)
  {
    /* Fill in parameters fro RRC_SYNC_IND command */
    cmd_ptr->cmd.rrc_sync_ind.cn_domain_id= cn_domain_id ;

    cmd_ptr->cmd.rrc_sync_ind.cause = cause_status;

    cmd_ptr->cmd.rrc_sync_ind.rab_id_present = FALSE;

#ifdef FEATURE_TDSCDMA_VOC_AMR_WB
    /* Fill in the codec type used in RRC_SYNC_IND*/
    cmd_ptr->cmd.rrc_sync_ind.rab_info.codec = RRC_CODEC_NONE;
#endif /*FEATURE_TDSCDMA_VOC_AMR_WB*/

#ifdef FEATURE_UMTS_VOICE_CIPHERING_IND
     #error code not present
#endif

#ifdef FEATURE_DUAL_SIM
    cmd_ptr->cmd.rrc_sync_ind.as_id = tdsrrc_get_as_id();
#endif

    /* Sends the command to MM */
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(cmd_ptr);
    }
    mm_put_cmd(cmd_ptr);

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sent RRC_SYNC_IND to MM");

  }    
  else   /* RRC Task failed to allocate memory */
  {
    ERR_FATAL("Unable to allocate memory for MM Cmd: %d",RRC_SYNC_IND,0,0);
  }     

} /* tdsrrcidt_send_sync_ind */
/*===========================================================================

FUNCTION tdsrrcidt_check_rrc_connection_status  

DESCRIPTION
  This function check RRC connection status by lloking at RCE substate.
  There is an additional check added to see if UE is OOS & RCE sub state
  is TDSRRCRCE_WAIT_FOR_SIG_CON_REL_COMPL,Call type is emergency call.
  In this case also return TRUE, since we need to put emergency call through
  so IDT will take care of handling required action for emergency call at later 
  part of code.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE : When RCE sub state is final & Call type is emergency call, with RCE substate 
  is TDSRRCRCE_WAIT_FOR_SIG_CON_REL_COMPL, UE is OOS.
  FALSE : Otherwise

===========================================================================*/

static boolean tdsrrcidt_check_rrc_connection_status
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{
  boolean status = FALSE;
  if((TDSRRCRCE_RRC_CONN_ESTABLISHED == tdsrrcrce_get_rrc_connection_status() ) 
      ||
     (((tdsrrcrce_get_rce_sub_state() == TDSRRCRCE_WAIT_FOR_RRC_TIMER_TO_EXPIRE)||
        (tdsrrcrce_get_rce_sub_state() == TDSRRCRCE_WAIT_FOR_SIG_CON_REL_COMPL))&&
      (cmd_ptr->cmd.open_sess_req.est_cause == RRC_EST_EMERGENCY_CALL)&&
      (TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_IDT ) )
     )
    )
  {
    /*1.If RCE is in Final sub state or 
     *2.RCE is in TDSRRCRCE_WAIT_FOR_SIG_CON_REL_COMPL, 
     *call type is emergency call & UE is OOS(Condition 2 will 
     *take UE out of stuck up case where UE is waiting for service
     *to send out SCRI messages,emergency call should be given 
     *a heigher priority ), return status as TRUE */
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"RCE sub state =%d,Est cause = %d, CU state = %d",tdsrrcrce_get_rce_sub_state(),
                                        cmd_ptr->cmd.open_sess_req.est_cause,
                                        tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_IDT ));
    status = TRUE;
  }

  return status;
}
/*===========================================================================

FUNCTION tdsrrcidt_validate_open_session_req  

DESCRIPTION
  This function validates TDSRRC_OPEN_SESSION_REQ command received from MM task.

DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcdt_status_e_type: It returns TDSRRCDT_SUCCESS if it is successfully validates 
  the TDSRRC_OPEN_SESSION_REQ command. Otherwise it returns TDSRRCDT_FAILURE_MINOR.

===========================================================================*/

static tdsrrcdt_status_e_type tdsrrcidt_validate_open_session_req
(
  tdsrrc_cmd_type *cmd_ptr,                 /* Pointer to the RRC Command */
  rrc_open_session_status_e_type *session_status
)
{
  tdsrrcdt_status_e_type status = TDSRRCDT_FAILURE_MINOR; 
  rrc_abort_cause_e_type  cause = RRC_MAX_ACCESS_FAILURE;
  *session_status=RRC_OPEN_SESS_FAILURE;
#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
   if(tdsrrc_csfb_extended_status == CSFB_CS_RB_RELEASED)
   {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"CSFB_HACK:csfb_extended_status=%d,Fail OPEN_SESSION_REQ",tdsrrc_csfb_extended_status);
      return (status);
   }
#endif
   /* Check whether RCR procedure is active or not */
   if (tdsrrcrcr_is_proc_active() == TRUE)
   {
     TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCR procedure is active");
     return (status);
   }
   
   /* Check whether RRC Connection Established or not */
   if (tdsrrcidt_check_rrc_connection_status(cmd_ptr) == FALSE)
   {
     TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC Conn is not established");
     return (status);
   }
   /* Check whether NAS selected PLMN ID in IDT is same as the one provided in dedicated OTA (if any) Refer 25.331 sec 8.1.8.2.
    * CCM updates the current requested PLMN ID in CSP internal data with the primary PLMN ID if CN information received in dedicated OTA 
    * contains the IE primary PLMN ID 
    */
   if(!tdsrrc_bcch_and_nas_plmns_match(tdsrrccsp_get_curr_nas_requested_plmn(),cmd_ptr->cmd.open_sess_req.selected_plmn_id))
   {
     TDSRRC_MSG0(MSG_LEGACY_HIGH,"Primary PLMN ID received in dedicated OTA is not same as NAS selected PLMN ID");
     return (status);
   }
   if(tdsrrcrce_validate_cell_access_status(cmd_ptr->cmd.open_sess_req.cn_domain_id,
                     cmd_ptr->cmd.open_sess_req.est_cause,
                     &cause) != TDSRRCRCE_SUCCESS)
   {
     /* Access Class is bard for this domain, So return from here*/
     *session_status = RRC_OPEN_SESS_FAILURE_AC_BARD;
     TDSRRC_MSG2(MSG_LEGACY_HIGH,"Access class Bard for CN doamin I.D = %d,Abort cause = %d",cmd_ptr->cmd.open_sess_req.cn_domain_id,cause);
     return (status);
   }

  /* Validate the Open Session Req primitive. Check to make sure that the
   * payload isn't too large or isn't empty.
   */
  
  if ((cmd_ptr->cmd.open_sess_req.nas_msg.length > TDSRRCDT_MAX_PAYLOAD) ||
      (cmd_ptr->cmd.open_sess_req.nas_msg.length == 0))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid payload size: %d",cmd_ptr->cmd.open_sess_req.nas_msg.length);
  }
  else /* Valid Payload */
  {
    /* check to see if the core n/w choice is RRC_GSM_MAP*/
    if(cmd_ptr->cmd.open_sess_req.intra_domain_nas_node_selector.
                corenetwork_choice != RRC_GSM_MAP)
    {
      /*lint -e793*/
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid CN choice %d", 
           cmd_ptr->cmd.open_sess_req.intra_domain_nas_node_selector.
           corenetwork_choice, 0, 0);
      /*lint +e793*/
      return(status);
    }

    /*check and see if we get a valid routing basis */
    if(cmd_ptr->cmd.open_sess_req.intra_domain_nas_node_selector.
                routing_info.routing_basis >=  RRC_MAX_ROUTING_BASIS)
    {
      /*lint -e793*/
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid Routing Basis choice %d", 
           cmd_ptr->cmd.open_sess_req.intra_domain_nas_node_selector.
           routing_info.routing_basis, 0, 0);
      /*lint +e793*/
      return(status);
    }    
    
    switch (cmd_ptr->cmd.open_sess_req.cn_domain_id)
    {
      case RRC_CS_DOMAIN_CN_ID :
        if (tdsrrcdt_int_data.cs_domain_open)
        {
          /* A signalling session to the CS domain is already open. Print a
           * message and send a failure back to MM.
           */
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"CS domain already open");

        }
        else
        {
          /* Identify the domain to which we will attempt to establish a
           * signalling session with
           */
          status = TDSRRCDT_SUCCESS;
        }
        break;

      case RRC_PS_DOMAIN_CN_ID :
        if (tdsrrcdt_int_data.ps_domain_open)
        {
          /* A signalling session to the PS domain is already open. Print a
           * message and send a failure back to MM.
           */
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"PS domain already open");

        }
        else
        {
        /* Identify the domain to which we will attempt to establish a
         * signalling session with
         */
          status = TDSRRCDT_SUCCESS;
        }
        break;

      default :
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid CN Domain ID %d",cmd_ptr->cmd.open_sess_req.cn_domain_id);
        break;
    }
  }
  *session_status=RRC_OPEN_SESS_SUCCESS;
  return (status);
} /* tdsrrcidt_validate_open_session_req */
/*===========================================================================

FUNCTION tdsrrcidt_save_open_session_req

DESCRIPTION
  This function saves the TDSRRC_OPEN_SESSION_REQ message contents 
  in the static variable saved_open_session_req
  
DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcdt_status_e_type.

===========================================================================*/
static tdsrrcdt_status_e_type tdsrrcidt_save_open_session_req
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{       
  
  unsigned char *payload;               /* Pointer to the MM payload */
  tdsrrcdt_status_e_type save_status = TDSRRCDT_FAILURE_MINOR;
                                        /* dt status to be returned */
  
   /* Set the CN Domain Id value */
  switch (cmd_ptr->cmd.open_sess_req.cn_domain_id)
  {
    case RRC_CS_DOMAIN_CN_ID :
      if((tdsrrcidt_cs_saved_open_session_req.open_sess_req_status == FALSE) &&
         (tdsrrcidt_cs_saved_open_session_req.mui_status == FALSE))
      {
        tdsrrcidt_cs_saved_open_session_req.open_session_req.cn_domain_id = RRC_CS_DOMAIN_CN_ID;
        /* Save the Intra Domain NAS Node selector */
        tdsrrcidt_cs_saved_open_session_req.open_session_req.intra_domain_nas_node_selector.corenetwork_choice = 
          cmd_ptr->cmd.open_sess_req.intra_domain_nas_node_selector.corenetwork_choice;
    
        tdsrrcidt_cs_saved_open_session_req.open_session_req.intra_domain_nas_node_selector.
          routing_info.routing_basis=
          cmd_ptr->cmd.open_sess_req.intra_domain_nas_node_selector.
          routing_info.routing_basis;
  
        tdsrrcidt_cs_saved_open_session_req.open_session_req.intra_domain_nas_node_selector.
          routing_info.routing_parameter[0]=
          cmd_ptr->cmd.open_sess_req.intra_domain_nas_node_selector.
          routing_info.routing_parameter[0];
  
        tdsrrcidt_cs_saved_open_session_req.open_session_req.intra_domain_nas_node_selector.
          routing_info.routing_parameter[1]=
          cmd_ptr->cmd.open_sess_req.intra_domain_nas_node_selector.
          routing_info.routing_parameter[1];
  
        tdsrrcidt_cs_saved_open_session_req.open_session_req.intra_domain_nas_node_selector.
          routing_info.entered_parameter=
          cmd_ptr->cmd.open_sess_req.intra_domain_nas_node_selector.
          routing_info.entered_parameter;
  
        /* Allocate a buffer for the payload. This memory is released by 
         * tdsrrcsend_send_ul_sdu().
         */
        payload = tdsrrc_malloc( cmd_ptr->cmd.open_sess_req.nas_msg.length);
    
        if (payload == NULL)
        {
          ERR_FATAL("malloc(%d) failure",cmd_ptr->cmd.open_sess_req.nas_msg.length,0,0);
        }
    
        /* Copy the payload from the MM primitive into the newly allocated memory
         */
        memscpy(payload, cmd_ptr->cmd.open_sess_req.nas_msg.length,
               cmd_ptr->cmd.open_sess_req.nas_msg.value, cmd_ptr->cmd.open_sess_req.nas_msg.length);
    
        /* Set the pointer to the payload and the length of the payload in the 
         * Initial Direct Transfer Message
        */
        tdsrrcidt_cs_saved_open_session_req.open_session_req.nas_msg.value = payload;
        tdsrrcidt_cs_saved_open_session_req.open_session_req.nas_msg.length = 
          (unsigned short) cmd_ptr->cmd.open_sess_req.nas_msg.length;
        /* Set status flag to TRUE */
        tdsrrcidt_cs_saved_open_session_req.open_sess_req_status = TRUE;
        tdsrrcidt_cs_saved_open_session_req.est_cause = cmd_ptr->cmd.open_sess_req.est_cause;

        /* Set fresh_start_value_needed flag to TRUE, So that 1st IDT carries START 
         *calculate according to 8.5.9   */
        tdsrrcidt_cs_saved_open_session_req.fresh_start_value_needed = TRUE;
  
       tdsrrcidt_cs_saved_open_session_req.open_session_req.call_type = cmd_ptr->cmd.open_sess_req.call_type;
  
        /*Save selected PLMN I.D*/
        tdsrrcidt_cs_saved_open_session_req.selected_plmn_id = cmd_ptr->cmd.open_sess_req.selected_plmn_id;
        save_status = TDSRRCDT_SUCCESS;
      }
      break;
    
    case RRC_PS_DOMAIN_CN_ID :
      if((tdsrrcidt_ps_saved_open_session_req.open_sess_req_status == FALSE)&&
         (tdsrrcidt_ps_saved_open_session_req.mui_status == FALSE))
      {
        tdsrrcidt_ps_saved_open_session_req.open_session_req.cn_domain_id = RRC_PS_DOMAIN_CN_ID;
        /* Save the Intra Domain NAS Node selector */
        tdsrrcidt_ps_saved_open_session_req.open_session_req.intra_domain_nas_node_selector.corenetwork_choice = 
          cmd_ptr->cmd.open_sess_req.intra_domain_nas_node_selector.corenetwork_choice;
    
        tdsrrcidt_ps_saved_open_session_req.open_session_req.intra_domain_nas_node_selector.
          routing_info.routing_basis=
          cmd_ptr->cmd.open_sess_req.intra_domain_nas_node_selector.
          routing_info.routing_basis;
  
        tdsrrcidt_ps_saved_open_session_req.open_session_req.intra_domain_nas_node_selector.
          routing_info.routing_parameter[0]=
          cmd_ptr->cmd.open_sess_req.intra_domain_nas_node_selector.
          routing_info.routing_parameter[0];
  
        tdsrrcidt_ps_saved_open_session_req.open_session_req.intra_domain_nas_node_selector.
          routing_info.routing_parameter[1]=
          cmd_ptr->cmd.open_sess_req.intra_domain_nas_node_selector.
          routing_info.routing_parameter[1];
  
        tdsrrcidt_ps_saved_open_session_req.open_session_req.intra_domain_nas_node_selector.
          routing_info.entered_parameter=
          cmd_ptr->cmd.open_sess_req.intra_domain_nas_node_selector.
          routing_info.entered_parameter;
  
        /* Allocate a buffer for the payload. This memory is released by 
         * tdsrrcsend_send_ul_sdu().
         */
        payload = tdsrrc_malloc( cmd_ptr->cmd.open_sess_req.nas_msg.length);
    
        if (payload == NULL)
        {
          ERR_FATAL("malloc(%d) failure",cmd_ptr->cmd.open_sess_req.nas_msg.length,0,0);
        }
    
        /* Copy the payload from the MM primitive into the newly allocated memory
         */
        memscpy(payload, 
               cmd_ptr->cmd.open_sess_req.nas_msg.length,
               cmd_ptr->cmd.open_sess_req.nas_msg.value,
               cmd_ptr->cmd.open_sess_req.nas_msg.length);
    
        /* Set the pointer to the payload and the length of the payload in the 
         * Initial Direct Transfer Message
         */
        tdsrrcidt_ps_saved_open_session_req.open_session_req.nas_msg.value = payload;
        tdsrrcidt_ps_saved_open_session_req.open_session_req.nas_msg.length = 
          (unsigned short) cmd_ptr->cmd.open_sess_req.nas_msg.length;

        /* Set status flag to TRUE */
        tdsrrcidt_ps_saved_open_session_req.open_sess_req_status = TRUE;
        tdsrrcidt_ps_saved_open_session_req.est_cause = cmd_ptr->cmd.open_sess_req.est_cause;

        /* Set fresh_start_value_needed flag to TRUE, So that 1st IDT carries START 
         *calculate according to 8.5.9   */
        tdsrrcidt_ps_saved_open_session_req.fresh_start_value_needed = TRUE;
  
        /*Save selected PLMN I.D*/
        tdsrrcidt_ps_saved_open_session_req.selected_plmn_id = cmd_ptr->cmd.open_sess_req.selected_plmn_id;
        save_status = TDSRRCDT_SUCCESS;
      }
      break;
    
    default :
      ERR_FATAL("Invalid Domain ID %d",cmd_ptr->cmd.open_sess_req.cn_domain_id,0,0);
      break;
  }
  
  /* If Open Session Req is successfully saved,check and suspend bplmn */
  if(TDSRRCDT_SUCCESS == save_status)
  {
    tdsrrcdt_check_and_suspend_bplmn(cmd_ptr->cmd.open_sess_req.est_cause);
  }
 
  return(save_status);
} /* tdsrrcidt_save_open_session_req */

/*===========================================================================

FUNCTION rrcidt_send_cs_initial_direct_tranfser_msg
DESCRIPTION
  This function sends the Initial Direct Transfer message on  Uplink DCCH for CS Domain
  
DEPENDENCIES
  None.

RETURN VALUE
    void

===========================================================================*/
static void tdsrrcidt_send_cs_initial_direct_transfer_msg
(
 void
)
{       
  tdsrrc_UL_DCCH_Message     *msg_ptr;         /* Pointer to uplink DCCH message */ 
  rlc_lc_id_type          ul_lc_id;         /* Uplink logical channel ID */
  tdsrrcsend_status_e_type send_status;        /* send status for UL message */
  uint8 cs_idt_mui;                                  /* Local variable for MUI */
  
  /* local variable to store address of rrc_seq90.ossBitString.value*/
  unsigned char * local_routingParameterValue_ptr = NULL;

  if(tdsrrcidt_cs_saved_open_session_req.open_sess_req_status == FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No IDT Message Saved for CS");
    return;
  }

  /* Determine the logical channel id for the Uplink DCCH
   */
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH,
    DCCH_DT_HIGH_PRI_RADIO_BEARER_ID, UE_MODE_ACKNOWLEDGED_DATA);
  
  /* Ensure that LCM found a valid logical channel id
   */
  if (ul_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
    msg_ptr = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc( 
      sizeof(struct tdsrrc_UL_DCCH_Message));
    
   /* Call error fatal if the memory allocation failed
    */
    if (msg_ptr == NULL)
    {
      ERR_FATAL("malloc(%d) failure",sizeof(struct tdsrrc_UL_DCCH_Message),0,0);
    }
    
   /* Identify this message as an Initial Direct Transfer Message
    */
    TDSRRCSET_MSG_TYPE(msg_ptr->message,
            T_tdsrrc_UL_DCCH_MessageType_initialDirectTransfer);
    
   /* Set bit_mask to zero since we aren't doing Measured Results On RACH.
    * There is a need  to look at SIB 11 and decide.
    */

    TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer); 
    
    if(tdsrrc_get_state() ==  TDSRRC_STATE_CELL_FACH)
    {
      /* Function call to Measurement Control & Report procedure. */
      if(TDSRRCMEAS_RACH_SUCCESSFUL == tdsrrcmeas_append_meas_rpt_on_rach(
         &msg_ptr->message.u.initialDirectTransfer.measuredResultsOnRACH, TDSRRC_PROCEDURE_IDT, FALSE))
      {
      /* if Measurement Control & Reportprocedure returns RRC_MEAS_RACH_SUCCESFUL,
         then enable the  bit_mask field */
         TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer,
             rrc_InitialDirectTransfer_,
             measuredResultsOnRACH);
       
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_MED,"RACH meas is not appended");
      }
    }
    
    /* Set the CN Domain Id value */
    
    msg_ptr->message.u.initialDirectTransfer.cn_DomainIdentity = 
          tdsrrc_CN_DomainIdentity_cs_domain;
      
    /* Set the Intra Domain NAS Node selector */
    TDSRRCSET_MSG_TYPE(msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version, 
          T_tdsrrc_IntraDomainNasNodeSelector_version_release99);


    /* Set the core network choice */
    switch(tdsrrcidt_cs_saved_open_session_req.open_session_req.intra_domain_nas_node_selector.
           corenetwork_choice)
    {
      case RRC_GSM_MAP:
        /*Allocate memory for release99 */ 
        msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version.u.release99 = 
         rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_IntraDomainNasNodeSelector_release99);
      
           
        if(msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version.u.release99 == NULL)
        {
          ERR_FATAL("Memory allocation failed",0,0,0);
        }
        
        TDSRRCSET_MSG_TYPE(msg_ptr->message.u.initialDirectTransfer.
           intraDomainNasNodeSelector.version.u.release99->cn_Type,
           T_tdsrrc_IntraDomainNasNodeSelector_cn_Type_gsm_Map_IDNNS);
        break;

      default:
        ERR_FATAL("Other than GSM MAP CN are not supported",0,0,0);
        break;

    }

    /*right now only RRC_GSM_MAP is supported add a condition for
      ansi later
     */
    if(msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version.u.release99 != NULL)
    {
      if((msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version.u.release99->cn_Type.u.gsm_Map_IDNNS = 
         rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_Gsm_map_IDNNS)) == NULL)
      {
        ERR_FATAL("Memory allocation failed",0,0,0);
      }  
    }
    else
    {
      ERR_FATAL("Invalid Params",0,0,0);
      }

    if(tdsrrcidt_cs_saved_open_session_req.open_session_req.intra_domain_nas_node_selector.
       corenetwork_choice == RRC_GSM_MAP)
    {
      switch( tdsrrcidt_cs_saved_open_session_req.open_session_req.intra_domain_nas_node_selector.
              routing_info.routing_basis)
      {
        case RRC_LOCAL_TMSI_OR_PTMSI:
          msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version.u.release99->
          cn_Type.u.gsm_Map_IDNNS->routingbasis.u.localPTMSI = 
             rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_Gsm_map_IDNNS_localPTMSI);         

          if(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.localPTMSI == NULL)
          {
            ERR_FATAL("Memory Allocation failure at localPTMSI",0,0,0);
          }
          
          TDSRRCSET_MSG_TYPE(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis,
          T_tdsrrc_Gsm_map_IDNNS_routingbasis_localPTMSI);

          msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.localPTMSI->routingparameter.
          numbits=TDSINTRA_DOMAIN_NAS_NODE_SELECTOR_LENGTH;

          local_routingParameterValue_ptr = msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.localPTMSI->routingparameter.
          data;

          break;

        case RRC_TMSI_OR_PTMSI_SAME_PLMN_AND_DIFFERENT_RA_OR_LA:
          msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version.u.release99->
          cn_Type.u.gsm_Map_IDNNS->routingbasis.u.tMSIofsamePLMN = 
            rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_Gsm_map_IDNNS_tMSIofsamePLMN); 
          
          if(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.tMSIofsamePLMN == NULL)
          {
            ERR_FATAL("Memory Allocation failure at tMSIofsamePLMN",0,0,0);
          }

          TDSRRCSET_MSG_TYPE(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis,
          T_tdsrrc_Gsm_map_IDNNS_routingbasis_tMSIofsamePLMN);

          msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.tMSIofsamePLMN->routingparameter.
          numbits=TDSINTRA_DOMAIN_NAS_NODE_SELECTOR_LENGTH;

          local_routingParameterValue_ptr = msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.tMSIofsamePLMN->routingparameter.
          data;

          break;

        case RRC_TMSI_OR_PTMSI_OF_DIFFERENT_PLMN:
          msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version.u.release99->
          cn_Type.u.gsm_Map_IDNNS->routingbasis.u.tMSIofdifferentPLMN = 
            rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_Gsm_map_IDNNS_tMSIofdifferentPLMN); 
          
          if(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.tMSIofdifferentPLMN == NULL)
          {
            ERR_FATAL("Memory Allocation failure at tMSIofdifferentPLMN",0,0,0);
          }
          
          TDSRRCSET_MSG_TYPE(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis,
          T_tdsrrc_Gsm_map_IDNNS_routingbasis_tMSIofdifferentPLMN);

          msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.tMSIofdifferentPLMN->routingparameter
          .numbits = TDSINTRA_DOMAIN_NAS_NODE_SELECTOR_LENGTH;

          local_routingParameterValue_ptr = msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.tMSIofdifferentPLMN->
          routingparameter.data;

          break;

        case RRC_IMSI_RESP_TO_IMSI_PAGING:
          msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version.u.release99->
          cn_Type.u.gsm_Map_IDNNS->routingbasis.u.iMSIresponsetopaging = 
            rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_Gsm_map_IDNNS_iMSIresponsetopaging);        

          if(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.iMSIresponsetopaging == NULL)
          {
            ERR_FATAL("Memory Allocation failure at iMSIresponsetopaging",0,0,0);
          }

          TDSRRCSET_MSG_TYPE(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis,
          T_tdsrrc_Gsm_map_IDNNS_routingbasis_iMSIresponsetopaging);

          msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.iMSIresponsetopaging->
          routingparameter.numbits=TDSINTRA_DOMAIN_NAS_NODE_SELECTOR_LENGTH;

          local_routingParameterValue_ptr = msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.iMSIresponsetopaging->
          routingparameter.data;
          break;

        case RRC_IMSI_UE_INITIATED_EVENT:
          msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version.u.release99->
          cn_Type.u.gsm_Map_IDNNS->routingbasis.u.iMSIcauseUEinitiatedEvent = 
            rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_Gsm_map_IDNNS_iMSIcauseUEinitiatedEvent);
          
          if(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.iMSIcauseUEinitiatedEvent == NULL)
          {
            ERR_FATAL("Memory Allocation failure at iMSIcauseUEinitiatedEvent",0,0,0);
          }

          TDSRRCSET_MSG_TYPE(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis,
          T_tdsrrc_Gsm_map_IDNNS_routingbasis_iMSIcauseUEinitiatedEvent);

          msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.iMSIcauseUEinitiatedEvent->
          routingparameter.numbits=TDSINTRA_DOMAIN_NAS_NODE_SELECTOR_LENGTH;

          local_routingParameterValue_ptr = msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.iMSIcauseUEinitiatedEvent->
          routingparameter.data;
          break;

        case RRC_IMEI:
          msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version.u.release99->
          cn_Type.u.gsm_Map_IDNNS->routingbasis.u.iMEI = 
            rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_Gsm_map_IDNNS_iMEI);          
          
          if(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.iMEI == NULL)
          {
            ERR_FATAL("Memory Allocation failure at iMEI",0,0,0);
          }

          TDSRRCSET_MSG_TYPE(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis,
          T_tdsrrc_Gsm_map_IDNNS_routingbasis_iMEI);

          msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.iMEI->routingparameter.numbits=
          TDSINTRA_DOMAIN_NAS_NODE_SELECTOR_LENGTH;

          local_routingParameterValue_ptr = msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.iMEI->routingparameter.data;
          break;

        default:
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unknown routing Basis received %d", 
               tdsrrcidt_cs_saved_open_session_req.open_session_req.intra_domain_nas_node_selector.
               routing_info.routing_basis, 0, 0);
          break;
      }

      if (local_routingParameterValue_ptr != NULL)
      {
        *(local_routingParameterValue_ptr) =
        (unsigned char ) (tdsrrcidt_cs_saved_open_session_req.open_session_req.
         intra_domain_nas_node_selector.routing_info.routing_parameter[0]);

        *(local_routingParameterValue_ptr + 1) = 
        (unsigned char ) (tdsrrcidt_cs_saved_open_session_req.open_session_req.
         intra_domain_nas_node_selector.routing_info.routing_parameter[1]);
      }
    }

    msg_ptr->message.u.initialDirectTransfer.
      intraDomainNasNodeSelector.version.u.release99->cn_Type.
      u.gsm_Map_IDNNS->dummy = 
      tdsrrcidt_cs_saved_open_session_req.open_session_req.intra_domain_nas_node_selector.
      routing_info.entered_parameter;

    /* Add 3A0 non-critical extensions */
    TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer,
      rrc_InitialDirectTransfer_,
      v3a0NonCriticalExtensions);

    /* Set no non-critical extensions for V3AO non-critical extensions */
    TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions);

    /* Append START value to the message */
    TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
      initialDirectTransfer_v3a0ext, 
      rrc_InitialDirectTransfer_v3a0ext_,
      start_Value);

    msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
      initialDirectTransfer_v3a0ext.start_Value.numbits = TDSRRC_START_VALUE_LENGTH;


    TDSRRC_MSG1(MSG_LEGACY_HIGH,"IDT fresh_start_value_needed flag status = %d",tdsrrcidt_cs_saved_open_session_req.fresh_start_value_needed);
    if(tdsrrcidt_cs_saved_open_session_req.fresh_start_value_needed == TRUE)
    {
      /*This is the first IDT, So calculate START value according to 8.5.9*/
      tdsrrcsmc_append_start_per_domain((byte *) (msg_ptr->message.u.initialDirectTransfer.
                                     v3a0NonCriticalExtensions.
                                     initialDirectTransfer_v3a0ext.start_Value.data),
                                     RRC_CS_DOMAIN_CN_ID,
                                     TDSRRCSMC_EXTRACT_AND_APPEND_START);
      /*Set fresh_start_value_needed flag to FALSE, as subsequent IDT should append START value*/
      tdsrrcidt_cs_saved_open_session_req.fresh_start_value_needed = FALSE;
    }
    else
    {
      /*This should be a subsequent IDT so append START value*/
      tdsrrcsmc_append_start_per_domain((byte *) (msg_ptr->message.u.initialDirectTransfer.
                                     v3a0NonCriticalExtensions.
                                     initialDirectTransfer_v3a0ext.start_Value.data),
                                     RRC_CS_DOMAIN_CN_ID,
                                     TDSRRCSMC_APPEND_START);
    }
  
    /* Copy the contents of Saved RRC_OPEN_SESS_REQ 
    */    
    memscpy(msg_ptr->message.u.initialDirectTransfer.nas_Message.data,
      tdsrrcidt_cs_saved_open_session_req.open_session_req.nas_msg.length,
      tdsrrcidt_cs_saved_open_session_req.open_session_req.nas_msg.value, 
      tdsrrcidt_cs_saved_open_session_req.open_session_req.nas_msg.length);

    msg_ptr->message.u.initialDirectTransfer.nas_Message.numocts = 
      tdsrrcidt_cs_saved_open_session_req.open_session_req.nas_msg.length;
    
    if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)) 
    {
      tdsrrcidt_fill_est_cause(msg_ptr, RRC_CS_DOMAIN_CN_ID);    
    }


   /* Send the CS Initial Direct Transfer Message on the Uplink DCCH
    */
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
    if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
    {
      if(tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH)
      {
        if(SUCCESS == tdsrrcifreq_append_meas_rpt_on_rach(&(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
          v590NonCriticalExtensions.v690NonCriticalExtensions.initialDirectTransfer_v690ext.measuredResultsOnRACHinterFreq),TDSRRC_PROCEDURE_IDT))
        {
          if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR((&msg_ptr->message.u.initialDirectTransfer),
              rrc_InitialDirectTransfer_,
              v3a0NonCriticalExtensions)))
          {
            TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer, 
              rrc_InitialDirectTransfer_,
              v3a0NonCriticalExtensions);
            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions);
            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.initialDirectTransfer_v3a0ext);
          }
          if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR((&msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions),
              rrc_InitialDirectTransfer_v3a0NonCriticalExtensions_,
              laterNonCriticalExtensions)))
          {
            TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions,
             rrc_InitialDirectTransfer_v3a0NonCriticalExtensions_,
             laterNonCriticalExtensions);
            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions);
          }
          if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR((&msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions),
              rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions_,
              v590NonCriticalExtensions)))
          {
            TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions,
             rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions_,
             v590NonCriticalExtensions);
            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
              laterNonCriticalExtensions.v590NonCriticalExtensions);
            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
              laterNonCriticalExtensions.v590NonCriticalExtensions.initialDirectTransfer_v590ext);
          }
          if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR((&msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions.v590NonCriticalExtensions),
              rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions_v590NonCriticalExtensions_,
              v690NonCriticalExtensions)))
          {
            TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
              laterNonCriticalExtensions.v590NonCriticalExtensions, 
             rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions_v590NonCriticalExtensions_,
             v690NonCriticalExtensions);
            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
              laterNonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions);
            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
              laterNonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.initialDirectTransfer_v690ext);
          }
          TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
             v590NonCriticalExtensions.v690NonCriticalExtensions.initialDirectTransfer_v690ext,
              rrc_InitialDirectTransfer_v690ext_IEs_,
              measuredResultsOnRACHinterFreq);
        }
      }
    }
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/

    if(tdsrrccsp_is_camped_cell_network_shared() == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS:N/W shared cell so include PLMN I.D in IDT");
      if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR((&msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions),
         rrc_InitialDirectTransfer_v3a0NonCriticalExtensions_,
         laterNonCriticalExtensions)))
      {
        /* Enable the laterNonCriticalExtension field in v3a0NoncriticalExtension structure*/
        TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions,
          rrc_InitialDirectTransfer_v3a0NonCriticalExtensions_,
          laterNonCriticalExtensions);
    
        TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
          laterNonCriticalExtensions);
      }
      if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR((&msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
                  laterNonCriticalExtensions),
              rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions_,
              v590NonCriticalExtensions)))
      {
        /* Enable the v590NonCriticalExtension field in laterNonCriticl in v3a0Non critical */
        TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
                  laterNonCriticalExtensions,
                  rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions_,
                  v590NonCriticalExtensions);

        TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
          laterNonCriticalExtensions.v590NonCriticalExtensions);
      }
      if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR((&msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
                  laterNonCriticalExtensions.v590NonCriticalExtensions.initialDirectTransfer_v590ext),
                  rrc_,
                  establishmentCause)))
      {
        /* Set bit mask properly if establishment I.E is not added in initialDirectTransfer_v590ext*/
        TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
          laterNonCriticalExtensions.v590NonCriticalExtensions.
          initialDirectTransfer_v590ext);
      }
      if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR((&msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
             laterNonCriticalExtensions.v590NonCriticalExtensions),
             rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions_v590NonCriticalExtensions_,
             v690NonCriticalExtensions)))
      {
        /* Enable the v690NonCriticalExtensions field in v590NonCriticalExtension in laterNonCriticl in v3a0Non critical */
        TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
         laterNonCriticalExtensions.v590NonCriticalExtensions,
         rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions_v590NonCriticalExtensions_,
         v690NonCriticalExtensions);

        TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
          laterNonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions);

        TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
            laterNonCriticalExtensions.v590NonCriticalExtensions.
            v690NonCriticalExtensions.initialDirectTransfer_v690ext);
      }

      /* Enable the rrc_InitialDirectTransfer_v690ext_IEs_plmn_Identity_present field in initialDirectTransfer_v690ext */
      TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
          laterNonCriticalExtensions.v590NonCriticalExtensions.
          v690NonCriticalExtensions.initialDirectTransfer_v690ext,
          rrc_InitialDirectTransfer_v690ext_IEs_,
          plmn_Identity);
      
      /* Fill PLMN I.D into OTA message which we received from NAS here */
      tdsrrcdt_fill_plmn_id_in_idt(&msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
        laterNonCriticalExtensions.v590NonCriticalExtensions.
        v690NonCriticalExtensions.initialDirectTransfer_v690ext.plmn_Identity,
        RRC_CS_DOMAIN_CN_ID);
    }

    send_status = tdsrrcsend_send_ul_sdu_with_mui(TDSRRC_PROCEDURE_IDT,msg_ptr,ul_lc_id,
                                      TDSRRCSEND_L2ACK_REQUIRED, &cs_idt_mui);
    
    if (send_status == TDSRRCSEND_SUCCESS)
    {
     /* Set the MUi and MUI status */
     tdsrrcidt_cs_saved_open_session_req.mui_status = TRUE;
     tdsrrcidt_cs_saved_open_session_req.open_sess_req_status = FALSE;
     tdsrrcidt_cs_saved_open_session_req.open_sess_cnf_required = TRUE;

     tdsrrcidt_cs_saved_open_session_req.mui = cs_idt_mui;
      
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Initial Direct Transfer Message Sent");

#ifdef FEATURE_TDSCDMA_MTPL_THERMAL_BACKOFF 
      if(RRC_EST_EMERGENCY_CALL == tdsrrcidt_cs_saved_open_session_req.est_cause) 
      {
        tdsrrcrce_set_rrc_est_cause_for_emergency_call();
      }
#endif	
      /* Wait for L2 ACK if there is no pending message for other Domain*/
      if(tdsrrcidt_ps_saved_open_session_req.open_sess_req_status == FALSE)
      {
        tdsrrcidt_substate = TDSRRCIDT_WAIT_FOR_L2ACK; 
      } 
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Error %d sending Initial Direct Xfer Msg",send_status);
      /* reset the state machine */
      tdsrrcidt_substate = TDSRRCIDT_INITIAL;

      /* Delete the saved CS TDSRRC_OPEN_SESSION_REQ */
      tdsrrcidt_delete_open_session_req(RRC_CS_DOMAIN_CN_ID);

    }
 
    /* The signalling session has been successfully opened. Set a flag to 
    * indicate that domain has been opened.
    */
    tdsrrcdt_int_data.cs_domain_open = TRUE;

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sig Conn for CS Domain is established");
             
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB 3 not found by LCM");

    /* Send RRC_OPEN_SESS_CNF Primitive to MM with Failure status */
    tdsrrcidt_send_open_session_cnf(RRC_OPEN_SESS_FAILURE,
      RRC_CS_DOMAIN_CN_ID);

    tdsrrcidt_delete_open_session_req(RRC_CS_DOMAIN_CN_ID);
  }
} /* rrcidt_send_cs_initial_direct_tranfser_msg */
/*===========================================================================

FUNCTION tdsrrcidt_send_ps_initial_direct_transfer_msg
DESCRIPTION
  This function sends the Initial Direct Transfer message on  Uplink DCCH for PS Domain
  
DEPENDENCIES
  None.

RETURN VALUE
    void

===========================================================================*/
static void tdsrrcidt_send_ps_initial_direct_transfer_msg
(
 void
)
{       
  tdsrrc_UL_DCCH_Message     *msg_ptr;         /* Pointer to uplink DCCH message */ 
  rlc_lc_id_type          ul_lc_id;         /* Uplink logical channel ID */
  tdsrrcsend_status_e_type send_status;        /* send status for UL message */
  uint8 ps_idt_mui;                                  /* Local variable for MUI */
  
  /* local variable to store address of rrc_seq90.ossBitString.value*/
  unsigned char * local_routingParameterValue_ptr = NULL;

  if(tdsrrcidt_ps_saved_open_session_req.open_sess_req_status == FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No IDT Message Saved for PS");
    return;
  }

  /* Determine the logical channel id for the Uplink DCCH
   */
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH,
    DCCH_DT_HIGH_PRI_RADIO_BEARER_ID, UE_MODE_ACKNOWLEDGED_DATA);
  
  /* Ensure that LCM found a valid logical channel id
   */
  if (ul_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
    msg_ptr = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc( 
      sizeof(struct tdsrrc_UL_DCCH_Message));
    
   /* Call error fatal if the memory allocation failed
    */
    if (msg_ptr == NULL)
    {
      ERR_FATAL("malloc(%d) failure",sizeof(struct tdsrrc_UL_DCCH_Message),0,0);
    }
    
   /* Identify this message as an Initial Direct Transfer Message
    */
    TDSRRCSET_MSG_TYPE(msg_ptr->message,
            T_tdsrrc_UL_DCCH_MessageType_initialDirectTransfer);
    
   /* Set bit_mask to zero since we aren't doing Measured Results On RACH.
    * There is a need  to look at SIB 11 and decide.
    */

    TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer); 
    
    if(tdsrrc_get_state() ==  TDSRRC_STATE_CELL_FACH)
    {
      /* Function call to Measurement Control & Report procedure. */
      if(TDSRRCMEAS_RACH_SUCCESSFUL == tdsrrcmeas_append_meas_rpt_on_rach(
         &msg_ptr->message.u.initialDirectTransfer.measuredResultsOnRACH, TDSRRC_PROCEDURE_IDT, FALSE))
      {
      /* if Measurement Control & Reportprocedure returns RRC_MEAS_RACH_SUCCESFUL,
         then enable the  bit_mask field */
         TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer,
          rrc_InitialDirectTransfer_,
          measuredResultsOnRACH);
       
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_MED,"RACH meas is not appended");
      }
    }
    
    /* Set the CN Domain Id value */
    
    msg_ptr->message.u.initialDirectTransfer.cn_DomainIdentity = 
          tdsrrc_CN_DomainIdentity_ps_domain;
      
    /* Set the Intra Domain NAS Node selector */
    TDSRRCSET_MSG_TYPE(msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version, 
           T_tdsrrc_IntraDomainNasNodeSelector_version_release99);


    /* Set the core network choice */
    switch(tdsrrcidt_ps_saved_open_session_req.open_session_req.intra_domain_nas_node_selector.
           corenetwork_choice)
    {
      case RRC_GSM_MAP:
        /*Allocate memory for release99 */ 
        msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version.u.release99 = 
        rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_IntraDomainNasNodeSelector_release99);
       
        
        if(msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version.u.release99 == NULL)
        {
          ERR_FATAL("Memory allocation failed",0,0,0);
        }
        TDSRRCSET_MSG_TYPE(msg_ptr->message.u.initialDirectTransfer.
        intraDomainNasNodeSelector.version.u.release99->cn_Type,
        T_tdsrrc_IntraDomainNasNodeSelector_cn_Type_gsm_Map_IDNNS);
        break;

      default:
        ERR_FATAL("Other than GSM MAP CN are not supported",0,0,0);
        break;

    }

    /*right now only RRC_GSM_MAP is supported add a condition for
      ansi later
     */
    if(msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version.u.release99 != NULL)
    {
      if((msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version.u.release99->cn_Type.u.gsm_Map_IDNNS = 
        rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_Gsm_map_IDNNS)) ==NULL)
      {
        ERR_FATAL("Memory allocation failed",0,0,0);
      }  
    }
    else
    {
      ERR_FATAL("Invalid Params",0,0,0);
      }

    if(tdsrrcidt_ps_saved_open_session_req.open_session_req.intra_domain_nas_node_selector.
       corenetwork_choice == RRC_GSM_MAP)
    {

      switch( tdsrrcidt_ps_saved_open_session_req.open_session_req.intra_domain_nas_node_selector.
              routing_info.routing_basis)
      {
        case RRC_LOCAL_TMSI_OR_PTMSI:
          msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version.u.release99->
          cn_Type.u.gsm_Map_IDNNS->routingbasis.u.localPTMSI = 
          rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_Gsm_map_IDNNS_localPTMSI);

          if(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.localPTMSI == NULL)
          {
            ERR_FATAL("Memory Allocation failure at localPTMSI",0,0,0);
          }
          
          TDSRRCSET_MSG_TYPE(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis,
          T_tdsrrc_Gsm_map_IDNNS_routingbasis_localPTMSI);

          msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.localPTMSI->routingparameter.
          numbits=TDSINTRA_DOMAIN_NAS_NODE_SELECTOR_LENGTH;

          local_routingParameterValue_ptr = msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.localPTMSI->routingparameter.
          data;

          break;

        case RRC_TMSI_OR_PTMSI_SAME_PLMN_AND_DIFFERENT_RA_OR_LA:
          msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version.u.release99->
          cn_Type.u.gsm_Map_IDNNS->routingbasis.u.tMSIofsamePLMN = 
          rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_Gsm_map_IDNNS_tMSIofsamePLMN);

          if(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.tMSIofsamePLMN == NULL)
          {
            ERR_FATAL("Memory Allocation failure at tMSIofsamePLMN",0,0,0);
          }

          TDSRRCSET_MSG_TYPE(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis,
          T_tdsrrc_Gsm_map_IDNNS_routingbasis_tMSIofsamePLMN);

          msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.tMSIofsamePLMN->routingparameter.
          numbits=TDSINTRA_DOMAIN_NAS_NODE_SELECTOR_LENGTH;

          local_routingParameterValue_ptr = msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.tMSIofsamePLMN->routingparameter.
          data;

          break;

        case RRC_TMSI_OR_PTMSI_OF_DIFFERENT_PLMN:
          msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version.u.release99->
          cn_Type.u.gsm_Map_IDNNS->routingbasis.u.tMSIofdifferentPLMN = 
          rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_Gsm_map_IDNNS_tMSIofdifferentPLMN);

          if(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.tMSIofdifferentPLMN == NULL)
          {
            ERR_FATAL("Memory Allocation failure at tMSIofdifferentPLMN",0,0,0);
          }
          
          TDSRRCSET_MSG_TYPE(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis,
          T_tdsrrc_Gsm_map_IDNNS_routingbasis_tMSIofdifferentPLMN);

          msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.tMSIofdifferentPLMN->routingparameter
          .numbits = TDSINTRA_DOMAIN_NAS_NODE_SELECTOR_LENGTH;

          local_routingParameterValue_ptr = msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.tMSIofdifferentPLMN->
          routingparameter.data;

          break;

        case RRC_IMSI_RESP_TO_IMSI_PAGING:
          msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version.u.release99->
          cn_Type.u.gsm_Map_IDNNS->routingbasis.u.iMSIresponsetopaging = 
          rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_Gsm_map_IDNNS_iMSIresponsetopaging);

          if(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.iMSIresponsetopaging == NULL)
          {
            ERR_FATAL("Memory Allocation failure at iMSIresponsetopaging",0,0,0);
          }

          TDSRRCSET_MSG_TYPE(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis,
          T_tdsrrc_Gsm_map_IDNNS_routingbasis_iMSIresponsetopaging);

          msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.iMSIresponsetopaging->
          routingparameter.numbits=TDSINTRA_DOMAIN_NAS_NODE_SELECTOR_LENGTH;

          local_routingParameterValue_ptr = msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.iMSIresponsetopaging->
          routingparameter.data;
          break;

        case RRC_IMSI_UE_INITIATED_EVENT:
          msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version.u.release99->
          cn_Type.u.gsm_Map_IDNNS->routingbasis.u.iMSIcauseUEinitiatedEvent = 
          rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_Gsm_map_IDNNS_iMSIcauseUEinitiatedEvent);

          if(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.iMSIcauseUEinitiatedEvent == NULL)
          {
            ERR_FATAL("Memory Allocation failure at iMSIcauseUEinitiatedEvent",0,0,0);
          }

          TDSRRCSET_MSG_TYPE(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis,
          T_tdsrrc_Gsm_map_IDNNS_routingbasis_iMSIcauseUEinitiatedEvent);

          msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.iMSIcauseUEinitiatedEvent->
          routingparameter.numbits=TDSINTRA_DOMAIN_NAS_NODE_SELECTOR_LENGTH;

          local_routingParameterValue_ptr = msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.iMSIcauseUEinitiatedEvent->
          routingparameter.data;
          break;

        case RRC_IMEI:
          msg_ptr->message.u.initialDirectTransfer.intraDomainNasNodeSelector.version.u.release99->
          cn_Type.u.gsm_Map_IDNNS->routingbasis.u.iMEI = 
          rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_Gsm_map_IDNNS_iMEI);
          
          if(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.iMEI == NULL)
          {
            ERR_FATAL("Memory Allocation failure at iMEI",0,0,0);
          }

          TDSRRCSET_MSG_TYPE(msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis,
          T_tdsrrc_Gsm_map_IDNNS_routingbasis_iMEI);

          msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.iMEI->routingparameter.numbits=
          TDSINTRA_DOMAIN_NAS_NODE_SELECTOR_LENGTH;

          local_routingParameterValue_ptr = msg_ptr->message.u.initialDirectTransfer.
          intraDomainNasNodeSelector.version.u.release99->cn_Type.
          u.gsm_Map_IDNNS->routingbasis.u.iMEI->routingparameter.data;
          break;

        default:
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unknown routing Basis received %d", 
               tdsrrcidt_ps_saved_open_session_req.open_session_req.intra_domain_nas_node_selector.
               routing_info.routing_basis, 0, 0);
          break;
      }

      if (local_routingParameterValue_ptr != NULL)
      {
       *(local_routingParameterValue_ptr) =
        (unsigned char ) (tdsrrcidt_ps_saved_open_session_req.open_session_req.
         intra_domain_nas_node_selector.routing_info.routing_parameter[0]);
       
        *(local_routingParameterValue_ptr + 1) = 
        (unsigned char ) (tdsrrcidt_ps_saved_open_session_req.open_session_req.
         intra_domain_nas_node_selector.routing_info.routing_parameter[1]);
      }
    }

    msg_ptr->message.u.initialDirectTransfer.
      intraDomainNasNodeSelector.version.u.release99->cn_Type.
      u.gsm_Map_IDNNS->dummy = 
      tdsrrcidt_ps_saved_open_session_req.open_session_req.intra_domain_nas_node_selector.
      routing_info.entered_parameter;

    /* Add 3A0 non-critical extensions */
    TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer,
      rrc_InitialDirectTransfer_,
      v3a0NonCriticalExtensions);

    /* Set no non-critical extensions for V3AO non-critical extensions */
    TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions);

    /* Append START value to the message */
    TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
      initialDirectTransfer_v3a0ext, 
      rrc_InitialDirectTransfer_v3a0ext_,
      start_Value);

    msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
      initialDirectTransfer_v3a0ext.start_Value.numbits = TDSRRC_START_VALUE_LENGTH;

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"IDT fresh_start_value_needed flag status = %d",tdsrrcidt_ps_saved_open_session_req.fresh_start_value_needed);
    if(tdsrrcidt_ps_saved_open_session_req.fresh_start_value_needed == TRUE)
    {
      /*This is the first IDT, So calculate START value according to 8.5.9*/
      tdsrrcsmc_append_start_per_domain((byte *) (msg_ptr->message.u.initialDirectTransfer.
                                     v3a0NonCriticalExtensions.
                                     initialDirectTransfer_v3a0ext.start_Value.data),
                                     RRC_PS_DOMAIN_CN_ID,
                                     TDSRRCSMC_EXTRACT_AND_APPEND_START);
      /*Set fresh_start_value_needed flag to FALSE, as subsequent IDT should append START value*/
      tdsrrcidt_ps_saved_open_session_req.fresh_start_value_needed = FALSE;
    }
    else
    {
      /*This should be a subsequent IDT so append START value*/
      tdsrrcsmc_append_start_per_domain((byte *) (msg_ptr->message.u.initialDirectTransfer.
                                     v3a0NonCriticalExtensions.
                                     initialDirectTransfer_v3a0ext.start_Value.data),
                                     RRC_PS_DOMAIN_CN_ID,
                                     TDSRRCSMC_APPEND_START);
    }
  
    /* Copy the contents of Saved RRC_OPEN_SESS_REQ 
    */    
    memscpy(msg_ptr->message.u.initialDirectTransfer.nas_Message.data,
      tdsrrcidt_ps_saved_open_session_req.open_session_req.nas_msg.length,
      tdsrrcidt_ps_saved_open_session_req.open_session_req.nas_msg.value, 
      tdsrrcidt_ps_saved_open_session_req.open_session_req.nas_msg.length);

    msg_ptr->message.u.initialDirectTransfer.nas_Message.numocts = 
      tdsrrcidt_ps_saved_open_session_req.open_session_req.nas_msg.length;

    if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)) 
    {
      tdsrrcidt_fill_est_cause(msg_ptr, RRC_PS_DOMAIN_CN_ID);    
    }

   /* Send the PS Initial Direct Transfer Message on the Uplink DCCH
    */
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
    if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
    {
      if(tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH)
      {
        if(SUCCESS == tdsrrcifreq_append_meas_rpt_on_rach(&(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
          v590NonCriticalExtensions.v690NonCriticalExtensions.initialDirectTransfer_v690ext.measuredResultsOnRACHinterFreq),TDSRRC_PROCEDURE_IDT))
        {
          if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR((&msg_ptr->message.u.initialDirectTransfer),
              rrc_InitialDirectTransfer_,
              v3a0NonCriticalExtensions)))
          {
            TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer, 
              rrc_InitialDirectTransfer_,
              v3a0NonCriticalExtensions);
            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions);
            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.initialDirectTransfer_v3a0ext);
          }
          if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR((&msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions),
              rrc_InitialDirectTransfer_v3a0NonCriticalExtensions_,
              laterNonCriticalExtensions)))
          {
            TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions,
             rrc_InitialDirectTransfer_v3a0NonCriticalExtensions_,
             laterNonCriticalExtensions);
            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions);
          }
          if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR((&msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions),
              rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions_,
              v590NonCriticalExtensions)))
          {
            TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions,
             rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions_,
             v590NonCriticalExtensions);
            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
              laterNonCriticalExtensions.v590NonCriticalExtensions);
            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
              laterNonCriticalExtensions.v590NonCriticalExtensions.initialDirectTransfer_v590ext);
          }
          if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR((&msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions.v590NonCriticalExtensions),
              rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions_v590NonCriticalExtensions_,
              v690NonCriticalExtensions)))
          {
            TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
              laterNonCriticalExtensions.v590NonCriticalExtensions, 
             rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions_v590NonCriticalExtensions_,
             v690NonCriticalExtensions);
            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
              laterNonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions);
            TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
              laterNonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.initialDirectTransfer_v690ext);
          }
          TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.laterNonCriticalExtensions.
             v590NonCriticalExtensions.v690NonCriticalExtensions.initialDirectTransfer_v690ext,
              rrc_InitialDirectTransfer_v690ext_IEs_,
              measuredResultsOnRACHinterFreq);
        }
      }
    }
#endif /* FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/
 
    if(tdsrrccsp_is_camped_cell_network_shared() == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS:N/W shared cell so include PLMN I.D in IDT");
      if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR((&msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions),
         rrc_InitialDirectTransfer_v3a0NonCriticalExtensions_,
         laterNonCriticalExtensions)))
      {
        /* Enable the laterNonCriticalExtension field in v3a0NoncriticalExtension structure*/
        TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions,
          rrc_InitialDirectTransfer_v3a0NonCriticalExtensions_,
          laterNonCriticalExtensions);
    
        TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
          laterNonCriticalExtensions);
      }
      if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR((&msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
                  laterNonCriticalExtensions),
              rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions_,
              v590NonCriticalExtensions)))
      {
        /* Enable the v590NonCriticalExtension field in laterNonCriticl in v3a0Non critical */
        TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
                  laterNonCriticalExtensions,
                  rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions_,
                  v590NonCriticalExtensions);

        TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
          laterNonCriticalExtensions.v590NonCriticalExtensions);
      }
      if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR((&msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
                  laterNonCriticalExtensions.v590NonCriticalExtensions.initialDirectTransfer_v590ext),
                  rrc_,
                  establishmentCause)))
      {
        /* Set bit mask properly if establishment I.E is not added in initialDirectTransfer_v590ext*/
        TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
          laterNonCriticalExtensions.v590NonCriticalExtensions.
          initialDirectTransfer_v590ext);
      }
      if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR((&msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
             laterNonCriticalExtensions.v590NonCriticalExtensions),
             rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions_v590NonCriticalExtensions_,
             v690NonCriticalExtensions)))
      {
        /* Enable the v690NonCriticalExtensions field in v590NonCriticalExtension in laterNonCriticl in v3a0Non critical */
        TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
         laterNonCriticalExtensions.v590NonCriticalExtensions,
         rrc_v3a0NonCriticalExtensions_laterNonCriticalExtensions_v590NonCriticalExtensions_,
         v690NonCriticalExtensions);

        TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
          laterNonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions);

        TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
          laterNonCriticalExtensions.v590NonCriticalExtensions.
            v690NonCriticalExtensions.initialDirectTransfer_v690ext);
      }

      /* Enable the rrc_InitialDirectTransfer_v690ext_IEs_plmn_Identity_present field in initialDirectTransfer_v690ext */
      TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
          laterNonCriticalExtensions.v590NonCriticalExtensions.
          v690NonCriticalExtensions.initialDirectTransfer_v690ext,
          rrc_InitialDirectTransfer_v690ext_IEs_,
          plmn_Identity);
      
      /* Fill PLMN I.D into OTA message which we received from NAS here */
      tdsrrcdt_fill_plmn_id_in_idt(&msg_ptr->message.u.initialDirectTransfer.v3a0NonCriticalExtensions.
          laterNonCriticalExtensions.v590NonCriticalExtensions.
          v690NonCriticalExtensions.initialDirectTransfer_v690ext.plmn_Identity,
          RRC_PS_DOMAIN_CN_ID);
    }
   
    send_status = tdsrrcsend_send_ul_sdu_with_mui(TDSRRC_PROCEDURE_IDT,msg_ptr,ul_lc_id,
                                      TDSRRCSEND_L2ACK_REQUIRED, &ps_idt_mui);
    
    if (send_status == TDSRRCSEND_SUCCESS)
    {
     /* Set the MUi and MUI status */
     tdsrrcidt_ps_saved_open_session_req.mui_status = TRUE;
     tdsrrcidt_ps_saved_open_session_req.open_sess_req_status = FALSE;
     tdsrrcidt_ps_saved_open_session_req.open_sess_cnf_required = TRUE;

     tdsrrcidt_ps_saved_open_session_req.mui = ps_idt_mui;
      
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Initial Direct Transfer Message Sent");

      /* Wait for L2 ACK if there is no pending message for other Domain*/
      if(tdsrrcidt_cs_saved_open_session_req.open_sess_req_status == FALSE)
      {
        tdsrrcidt_substate = TDSRRCIDT_WAIT_FOR_L2ACK; 
      }
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Error %d sending Initial Direct Xfer Msg",send_status);
      /* reset the state machine */
      tdsrrcidt_substate = TDSRRCIDT_INITIAL;

      /* Delete the saved CS TDSRRC_OPEN_SESSION_REQ */
      tdsrrcidt_delete_open_session_req(RRC_PS_DOMAIN_CN_ID);

    }
 
    /* The signalling session has been successfully opened. Set a flag to 
    * indicate that domain has been opened.
    */
    tdsrrcdt_int_data.ps_domain_open = TRUE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sig Conn for PS Domain is established");
             
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB 3 not found by LCM");

    /* Send RRC_OPEN_SESS_CNF Primitive to MM with Failure status */
    tdsrrcidt_send_open_session_cnf(RRC_OPEN_SESS_FAILURE,
      RRC_PS_DOMAIN_CN_ID);

    tdsrrcidt_delete_open_session_req(RRC_PS_DOMAIN_CN_ID);
  }
} /* tdsrrcidt_send_ps_initial_direct_transfer_msg */
/*===========================================================================

FUNCTION tdsrrcdt_send_cell_update_initiate_req

DESCRIPTION
  This function send the cell update initiate request if we are in
  URA_PCH or CELL_PCH states.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
static void tdsrrcdt_send_cell_update_initiate_req
(
  tdsrrc_cell_update_cause_e_type cause
)
{
  tdsrrc_cmd_type *int_cmd_ptr;
  //This initiates a CELL Update procedure and enters the substate
  //WAIT_FOR_CELL_UPDATE_COMP_CNF. 
  if( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {     
    /* Fill in the command parameters for TDSRRC_CELL_UPDATE_INITIATE_REQ */
    int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CELL_UPDATE_INITIATE_REQ;
    int_cmd_ptr->cmd.initiate_cell_update.cause = cause;
    int_cmd_ptr->cmd.initiate_cell_update.cu_complete_cnf_required = TRUE;
    int_cmd_ptr->cmd.initiate_cell_update.procedure = TDSRRC_PROCEDURE_IDT;
        
    tdsrrc_put_int_cmd(int_cmd_ptr);             
  }
  else
  {
    ERR_FATAL("Unable to allocate memmory for: %d cmd",TDSRRC_CELL_UPDATE_INITIATE_REQ,0,0);
  }
}/*tdsrrcdt_send_cell_update_initiate_req*/

/*===========================================================================

FUNCTION tdsrrcidt_send_nas_data_ind

DESCRIPTION
  This function sends TDSRR_NAS_DATA_IND if there is any NAS message pending
  to transmitted or pending to be acknowledged from UTRAN.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
static void tdsrrcidt_send_nas_data_ind
(
  void
)
{
  tdsrr_nas_data_ind_type nas_data_ind;    /* Local variable to store
                                           NAS message to be sent to RR */

  memset(&nas_data_ind,0,sizeof(tdsrr_nas_data_ind_type)); 
                                           
  switch(tdsrrcidt_substate)
  {
    case TDSRRCIDT_INITIAL:
      /* No Pending NAS message. Do not send a NULL message here because it will
         be done at the end of sending UDT messages. RR expects only one NULL
         message at the end of all NAS DATA IND calls (after both IDT and UDT) */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No Pending IDT messages");
      break;

    case TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF:                               
    case TDSRRCIDT_WAIT_FOR_L2ACK:

      /* Send CS saved IDT, if there is one */ 
      if(((tdsrrcidt_cs_saved_open_session_req.mui_status == TRUE) ||
          (tdsrrcidt_cs_saved_open_session_req.open_sess_req_status == TRUE)) && 
         (tdsrrcidt_cs_saved_open_session_req.open_session_req.nas_msg.length != 0))
      {
        if(tdsrrcidt_cs_saved_open_session_req.open_session_req.nas_msg.length > 255)
        {
          ERR_FATAL("Len of NAS Msg is > 255",0,0,0);
        }

        nas_data_ind.nas_data.message_length = (uint8)
          tdsrrcidt_cs_saved_open_session_req.open_session_req.nas_msg.length;

        memscpy(nas_data_ind.nas_data.payload,
               nas_data_ind.nas_data.message_length,
               tdsrrcidt_cs_saved_open_session_req.open_session_req.nas_msg.value,
               tdsrrcidt_cs_saved_open_session_req.open_session_req.nas_msg.length);

        /* Send the message to RR */
        tdsrr_send_rrc_message_to_rr_internal((void *)(&nas_data_ind),
                                   TDSRR_NAS_DATA_IND,
                                   sizeof(tdsrr_nas_data_ind_type));
      
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sent TDSRR_NAS_DATA_IND message length: %d",nas_data_ind.nas_data.message_length);
      }

      /* Send PS saved IDT, if there is one */ 
      if(((tdsrrcidt_ps_saved_open_session_req.mui_status == TRUE) ||
          (tdsrrcidt_ps_saved_open_session_req.open_sess_req_status == TRUE)) && 
         (tdsrrcidt_ps_saved_open_session_req.open_session_req.nas_msg.length != 0))
      {
        if(tdsrrcidt_ps_saved_open_session_req.open_session_req.nas_msg.length > 255)
        {
          ERR_FATAL("Len of NAS Msg is > 255",0,0,0);
        }

        nas_data_ind.nas_data.message_length = (uint8)
          tdsrrcidt_ps_saved_open_session_req.open_session_req.nas_msg.length;

        memscpy(nas_data_ind.nas_data.payload,
               nas_data_ind.nas_data.message_length,
               tdsrrcidt_ps_saved_open_session_req.open_session_req.nas_msg.value,
               tdsrrcidt_ps_saved_open_session_req.open_session_req.nas_msg.length);

        /* Send the message to RR */
        tdsrr_send_rrc_message_to_rr_internal((void *)(&nas_data_ind),
                                   TDSRR_NAS_DATA_IND,
                                   sizeof(tdsrr_nas_data_ind_type));

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sent TDSRR_NAS_DATA_IND message length: %d",nas_data_ind.nas_data.message_length);
      }

      /* Delete saved IDTs of both domains since we have sent it to RR */
      tdsrrcidt_delete_open_session_req(RRC_CS_DOMAIN_CN_ID); 
      tdsrrcidt_delete_open_session_req(RRC_PS_DOMAIN_CN_ID); 

      /* Do not send a NULL message here because it will be done at the end of sending 
         UDT messages. RR expects only one NULL message at the end of all NAS DATA IND 
         calls (after both IDT and UDT) */
      break;

    default:  
      ERR_FATAL("Invalid RRCIDT substate: %d ",tdsrrcidt_substate,0,0);
      break;
   }

}/*tdsrrcidt_send_nas_data_ind*/
/*===========================================================================

FUNCTION tdsrrcidt_process_idt_initial_substate

DESCRIPTION
  This function processes TDSRRC_OPEN_SESSION_REQ event that is  dispatched
  to Initial Direct Transfer procedure in TDSRRCIDT_INITIAL substate.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

static void tdsrrcidt_process_idt_initial_substate
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{
  rrc_open_session_status_e_type session_status = RRC_OPEN_SESS_SUCCESS;
  sys_modem_as_id_e_type as_id = tdsrrc_get_as_id();

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    /* this happens when a state change ind is received in idle state.
     * do nothing when it happens
     */
    case TDSRRC_OPEN_SESSION_REQ:
      if((TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_IDT ) )&&
        (tdsrrcidt_check_rrc_connection_status(cmd_ptr) == TRUE))
      {
        if(tdsis_ue_oos_and_em_call_attempted(cmd_ptr, TDSRRC_PROCEDURE_IDT))
        {
          tdsrrcidt_substate = TDSRRCIDT_INITIAL;  
          return;
        }
      }
      /* Before opening a new signalling session, be sure that there isn't one 
       * alreay open to the requested domain.  Also validate contents of the
       * open session request
       */
      if(TDSRRCDT_SUCCESS != tdsrrcidt_validate_open_session_req(cmd_ptr,&session_status))
      {
        if(session_status == RRC_OPEN_SESS_FAILURE_AC_BARD)
        {
          /* Access Class is bard for this domain,So send RRC_OPEN_SESS_CNF Primitive
             to MM with Failure status as RRC_OPEN_SESS_FAILURE_AC_BARD */
          tdsrrcidt_send_open_session_cnf(RRC_OPEN_SESS_FAILURE_AC_BARD,
                                    cmd_ptr->cmd.open_sess_req.cn_domain_id);
        }
        else
        {
          /* Send RRC_OPEN_SESS_CNF Primitive to MM with Failure status */
          tdsrrcidt_send_open_session_cnf(RRC_OPEN_SESS_FAILURE,
                                    cmd_ptr->cmd.open_sess_req.cn_domain_id);
        }
        return;
      }
    
      /* save the contents of the open_session_req message. will be used
       * later to send out the idt message
       */
      if(TDSRRCDT_SUCCESS != tdsrrcidt_save_open_session_req(cmd_ptr))
      {
        /* Send RRC_OPEN_SESS_CNF Primitive to MM with Failure status */
        tdsrrcidt_send_open_session_cnf(RRC_OPEN_SESS_FAILURE,
                                    cmd_ptr->cmd.open_sess_req.cn_domain_id);
        return;
      }
    
      switch(tdsrrc_get_state())
      {
        case TDSRRC_STATE_URA_PCH:
        case TDSRRC_STATE_CELL_PCH: 
          if(TDSRRCCU_COMPLETED == tdsrrccu_get_cell_update_complete_status(TDSRRC_PROCEDURE_IDT))
          {
            tdsrrcidt_set_est_cause(cmd_ptr->cmd.open_sess_req.est_cause, 
                      TDSRRC_PROCEDURE_CU, cmd_ptr->cmd.open_sess_req.cn_domain_id);
            switch (cmd_ptr->cmd.open_sess_req.cn_domain_id)
            {
              case RRC_CS_DOMAIN_CN_ID :
                tdsrrcidt_cs_saved_open_session_req.cu_init_by_idt = TRUE;
              break;

                case RRC_PS_DOMAIN_CN_ID :
                tdsrrcidt_ps_saved_open_session_req.cu_init_by_idt = TRUE;
              break;

              default:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid CN Domain");
              break;
            }
            /* if there is not cell update in progress initiate */
            /* cell update with uplink data transmission        */
            tdsrrcdt_send_cell_update_initiate_req(TDSRRC_UL_DATA_TRANSMISSION);
            /* Wait until Cell update procedure is completed */
            tdsrrcidt_substate = TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;  
          }
          else
          {
            if(tdsis_ue_oos_and_em_call_attempted(cmd_ptr, TDSRRC_PROCEDURE_IDT))
            {
              tdsrrcidt_substate = TDSRRCIDT_INITIAL;  
            }
            else
            {
              tdsrrcidt_set_est_cause(cmd_ptr->cmd.open_sess_req.est_cause, TDSRRC_PROCEDURE_IDT,
                  cmd_ptr->cmd.open_sess_req.cn_domain_id);
 
             /* Wait until Cell update procedure is completed */
              tdsrrcidt_substate = TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;  
            }
          }
          break;

        case TDSRRC_STATE_CELL_FACH:
        case TDSRRC_STATE_CELL_DCH:
          //This queries whether a CELL Update procedure is active.If Cell Update
          //is active, it registers for the complete and enters the substate
          //RRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF. 
          if (cmd_ptr->cmd.open_sess_req.cn_domain_id == RRC_PS_DOMAIN_CN_ID)
          {
              /* Both PS and CS hasn't receive the RAB setup. */
              if ((mn_voice_call_vote_for_reselection(as_id) == NAS_VOTE_FOR_RESEL_FALSE)&&
                  (sm_ps_call_vote_for_reselection(as_id) == NAS_VOTE_FOR_RESEL_FALSE))
              {  
                if (tdsrrc_nv_hold_ps_tmr_value > 0)
                {
                 TDSRRC_MSG1(MSG_LEGACY_HIGH,"Hold PS: hold %d ms", tdsrrc_nv_hold_ps_tmr_value*10);
                 tdsrrctmr_start_timer(TDSRRCTMR_HOLD_PS_TIMER, tdsrrc_nv_hold_ps_tmr_value*10);
                 tdsrrc_hold_ps_tmr_expired = FALSE;
                }
                else
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Hold PS: don't hold PS");
                }
              }
          }
          if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_IDT ) )
          {
            if(tdsis_ue_oos_and_em_call_attempted(cmd_ptr, TDSRRC_PROCEDURE_IDT))
            {
              tdsrrcidt_substate = TDSRRCIDT_INITIAL;  
            }
            else
            {
              tdsrrcidt_set_est_cause(cmd_ptr->cmd.open_sess_req.est_cause, 
                  TDSRRC_PROCEDURE_IDT, cmd_ptr->cmd.open_sess_req.cn_domain_id);

              /* Wait until Cell update procedure is completed */
              tdsrrcidt_substate = TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;  
            }
          }
          else /* Cell Update procedure is inactive */
          {
            tdsrrcidt_set_est_cause(cmd_ptr->cmd.open_sess_req.est_cause, 
                  TDSRRC_PROCEDURE_IDT, cmd_ptr->cmd.open_sess_req.cn_domain_id);
            /* Build and send the Initial Direct Transfer Msg to Send Chain */
            if(tdsrrcidt_cs_saved_open_session_req.open_sess_req_status == TRUE)
            {
              tdsrrcidt_send_cs_initial_direct_transfer_msg();
            }
            if((tdsrrcidt_ps_saved_open_session_req.open_sess_req_status == TRUE)&&(tdsrrc_hold_ps_tmr_expired == TRUE))
            {
              tdsrrcidt_send_ps_initial_direct_transfer_msg();
            }
          }
          break;
    
        default:
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d for IDT in substate:%d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcidt_substate);
        break;
      }
    
      break;

    case TDSRRC_NAS_DATA_TRANSFER_IND:
      tdsrrcidt_send_nas_data_ind();   
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d for IDT in substate:%d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcidt_substate);
      break;
  }/*end of switch stmt*/
} /* tdsrrcidt_process_idt_initial_substate */

/*===========================================================================

FUNCTION tdsrrcidt_process_idt_wait_for_cell_update_comp_cnf_substate

DESCRIPTION
  This function processes all events that are  dispatched
  to Initial Direct Transfer procedure in TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF
  substate.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

static void tdsrrcidt_process_idt_wait_for_cell_update_comp_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{
  rrc_open_session_status_e_type session_status = RRC_OPEN_SESS_SUCCESS;
  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
      tdsrrcidt_cs_saved_open_session_req.cu_init_by_idt = FALSE;
      tdsrrcidt_ps_saved_open_session_req.cu_init_by_idt = FALSE;

      switch(tdsrrc_get_state())
      {
        case TDSRRC_STATE_URA_PCH:
        case TDSRRC_STATE_CELL_PCH:   

          if(TDSRRCCU_COMPLETED == 
                 tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_IDT ) )
          {
              /* change the owner of establishment cause to CU */
            tdsrrcidt_change_owner_est_cause(TDSRRC_PROCEDURE_CU);
            if(tdsrrcidt_is_idt_buffered(RRC_CS_DOMAIN_CN_ID) == TRUE)
            {
              tdsrrcidt_cs_saved_open_session_req.cu_init_by_idt = TRUE;
            }
            if(tdsrrcidt_is_idt_buffered(RRC_PS_DOMAIN_CN_ID) == TRUE)
            {
              tdsrrcidt_ps_saved_open_session_req.cu_init_by_idt = TRUE;
            }
            /* initiate cell update if it not already running */
            /* with cause uplink data transmission */
            tdsrrcdt_send_cell_update_initiate_req(TDSRRC_UL_DATA_TRANSMISSION);
          }

          /* Wait until Cell update procedure is completed */
          tdsrrcidt_substate = TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;  
          return;

        default:
                ;
          /* for other states continue */
      }
      tdsrrcidt_check_if_transmission_success(TDSRRC_PROCEDURE_CU);
      tdsrrcidt_reset_est_cause(TDSRRC_PROCEDURE_CU);

      /* Build and send the Initial Direct Transfer Msg to Send Chain */
      if(tdsrrcidt_cs_saved_open_session_req.open_sess_req_status == TRUE)
      {
        tdsrrcidt_send_cs_initial_direct_transfer_msg();
      }
      if(tdsrrcidt_ps_saved_open_session_req.open_sess_req_status == TRUE)
      {
        tdsrrcidt_send_ps_initial_direct_transfer_msg();
      }

      /* Reset the state machine */
      if((tdsrrcidt_cs_saved_open_session_req.mui_status == FALSE)  &&
         (tdsrrcidt_ps_saved_open_session_req.mui_status == FALSE) 
         )
      {
        tdsrrcidt_substate = TDSRRCIDT_INITIAL;  
      }
      break;

    case TDSRRC_OPEN_SESSION_REQ:
      if((TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_IDT ) )&&
        (tdsrrcidt_check_rrc_connection_status(cmd_ptr) == TRUE))
      {
        if(tdsis_ue_oos_and_em_call_attempted(cmd_ptr, TDSRRC_PROCEDURE_IDT))
        {
          tdsrrcidt_substate = TDSRRCIDT_INITIAL;  
          return;
        }
      }
      /* Before opening a new signalling session, be sure that there isn't one 
       * alreay open to the requested domain.  Also validate contents of the
       * open session request
       */
      if(TDSRRCDT_SUCCESS != tdsrrcidt_validate_open_session_req(cmd_ptr,&session_status))
      {
        if(session_status == RRC_OPEN_SESS_FAILURE_AC_BARD)
        {
          /* Access Class is bard for this domain,So send RRC_OPEN_SESS_CNF Primitive
             to MM with Failure status as RRC_OPEN_SESS_FAILURE_AC_BARD */
          tdsrrcidt_send_open_session_cnf(RRC_OPEN_SESS_FAILURE_AC_BARD,
                                    cmd_ptr->cmd.open_sess_req.cn_domain_id);
        }
        else
        {
          /* Send RRC_OPEN_SESS_CNF Primitive to MM with Failure status */
          tdsrrcidt_send_open_session_cnf(RRC_OPEN_SESS_FAILURE,
                                    cmd_ptr->cmd.open_sess_req.cn_domain_id);
        }
        return;
      }
    
      /* save the contents of the open_session_req message. will be used
       * later to send out the idt message
       */
      if(TDSRRCDT_SUCCESS != tdsrrcidt_save_open_session_req(cmd_ptr))
      {
        /* Send RRC_OPEN_SESS_CNF Primitive to MM with Failure status */
        tdsrrcidt_send_open_session_cnf(RRC_OPEN_SESS_FAILURE,
                                    cmd_ptr->cmd.open_sess_req.cn_domain_id);
        return;
      }

      if(tdsis_ue_oos_and_em_call_attempted(cmd_ptr, TDSRRC_PROCEDURE_IDT))
      {
        tdsrrcidt_substate = TDSRRCIDT_INITIAL;  
      }
      else  
      {
         tdsrrcidt_set_est_cause(cmd_ptr->cmd.open_sess_req.est_cause, 
                TDSRRC_PROCEDURE_IDT, cmd_ptr->cmd.open_sess_req.cn_domain_id);
      }

      break;


    case TDSRRC_NAS_DATA_TRANSFER_IND:
      tdsrrcidt_send_nas_data_ind();  
      /* Reset the state machine */   
      tdsrrcidt_delete_open_session_req(RRC_CS_DOMAIN_CN_ID);  
      tdsrrcidt_delete_open_session_req(RRC_PS_DOMAIN_CN_ID); 
      tdsrrcidt_reset_est_cause(TDSRRC_PROCEDURE_CU);       
      tdsrrcidt_substate = TDSRRCIDT_INITIAL; 
      break;


    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d for IDT in substate:%d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcidt_substate);
      break;
  }
 
} /* tdsrrcidt_process_idt_wait_for_cell_update_comp_cnf_substate */
/*===========================================================================

FUNCTION tdsrrcidt_is_saved_open_session_req

DESCRIPTION
  This function returns TRUE if there is saved open_seesion_req for requested CN domain, and FALSE otherwise.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE:  There is saved open_seesion_req for requested CN domain
  FALSE: Otherwise

===========================================================================*/
boolean tdsrrcidt_is_saved_open_session_req
(
  rrc_cn_domain_identity_e_type cn_domain
)
{
  boolean status = FALSE;

  if((cn_domain == RRC_CS_DOMAIN_CN_ID) &&
     (tdsrrcidt_cs_saved_open_session_req.open_sess_req_status == TRUE) &&
     (tdsrrcidt_cs_saved_open_session_req.open_session_req.cn_domain_id == RRC_CS_DOMAIN_CN_ID)
    )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saved CS open_sess_req");
    status = TRUE;
  }
  else if((cn_domain == RRC_PS_DOMAIN_CN_ID) &&
          (tdsrrcidt_ps_saved_open_session_req.open_sess_req_status == TRUE) &&
          (tdsrrcidt_ps_saved_open_session_req.open_session_req.cn_domain_id == RRC_PS_DOMAIN_CN_ID)
         )
   {
     TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saved PS open_sess_req");
     status = TRUE;
   }
  return status;
}
/*===========================================================================

FUNCTION tdsrrcidt_is_idt_waiting_for_l2ack_per_domain

DESCRIPTION
  This function returns TRUE/FALSE depending if L2_ACK is pending for
  CS or PS domain which is passed as a formal parameter to the fn.
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE:  The CN domain requested is waiting for L2_ACK
  FALSE: The CN domain requested is not waiting for L2_ACK 

===========================================================================*/
boolean tdsrrcidt_is_idt_waiting_for_l2ack_per_domain
(
  rrc_cn_domain_identity_e_type cn_domain
)
{
  /*return status to indicate whether OPEN_SESS_CNF is to be sent*/
  boolean status = FALSE;

  /*check the domain and also if mui_status is TRUE i.e. we are waiting for
    L2_ACK for the domain requested
   */
  if((cn_domain == RRC_CS_DOMAIN_CN_ID) && 
     (tdsrrcidt_cs_saved_open_session_req.mui_status == TRUE))
  {
    tdsrrcidt_clear_idt_data_for_race_conditions(cn_domain);
    status = TRUE;
  }
  else if((cn_domain == RRC_PS_DOMAIN_CN_ID) && 
          (tdsrrcidt_ps_saved_open_session_req.mui_status == TRUE))
  {
    tdsrrcidt_clear_idt_data_for_race_conditions(cn_domain);
    status = TRUE;
  }

  return status;
}
/*===========================================================================

FUNCTION tdsrrcidt_process_idt_wait_for_l2ack_substate

DESCRIPTION
  This function processes all events that are  dispatched
  to Initial Direct Transfer procedure in TDSRRCIDT_WAIT_FOR_L2ACK
  substate.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

static void tdsrrcidt_process_idt_wait_for_l2ack_substate
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{
  rrc_open_session_status_e_type session_status = RRC_OPEN_SESS_SUCCESS;
  sys_modem_as_id_e_type as_id = tdsrrc_get_as_id();

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_DOWNLINK_L2ACK_IND:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"CS domain IDT status: mui_status %d,mui %d",tdsrrcidt_cs_saved_open_session_req.mui_status,tdsrrcidt_cs_saved_open_session_req.mui);

      TDSRRC_MSG2(MSG_LEGACY_HIGH,"PS domain IDT status: mui_status %d,mui %d",tdsrrcidt_ps_saved_open_session_req.mui_status,tdsrrcidt_ps_saved_open_session_req.mui);

      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Received IDT L2-ACK status %d mui %d",cmd_ptr->cmd.downlink_l2ack_ind.status,cmd_ptr->cmd.downlink_l2ack_ind.mui);

      /* Send RRC_OPEN_SESSION_CNF to MM */
      if((tdsrrcidt_cs_saved_open_session_req.mui_status == TRUE) &&
         (tdsrrcidt_cs_saved_open_session_req.mui == cmd_ptr->cmd.downlink_l2ack_ind.mui))
      {
        /*if L2_ACK for SMC is not received before L2_ACK for IDT,
          send open_sess_cnf here, else open_session_cnf would have
          been sent by SMC just before sending SYNC_IND
         */
        if(tdsrrcidt_cs_saved_open_session_req.open_sess_cnf_required == TRUE)
        {
		  /* Check and send any blocked SCRR for this domain */
	      boolean cs_scrr_active = tdsrrcdt_check_and_send_pending_scrr(RRC_CS_DOMAIN_CN_ID);

          /* Send a cnf to NAS if L2 ACK succeeded.Also if there is a
		     pending SCRR for this domain,send a Cnf to NAS as we will
			 not be pursuing the retransmissions */
          if((SUCCESS == cmd_ptr->cmd.downlink_l2ack_ind.status)||
			 (TRUE == cs_scrr_active))
          {
            tdsrrcidt_send_open_session_cnf(RRC_OPEN_SESS_SUCCESS,
                                         RRC_CS_DOMAIN_CN_ID);
            tdsrrcidt_cs_saved_open_session_req.open_sess_cnf_required = FALSE;

            /* Delete the saved TDSRRC_OPEN_SESSION_REQ */
            tdsrrcidt_delete_open_session_req(RRC_CS_DOMAIN_CN_ID);

            TDSRRC_MSG3(MSG_LEGACY_HIGH,"Security NV settings: Integrity %d, Ciphering_status %d, Fake %d",
                                         tdsnv_integrity_enabled,
                                         tdsnv_ciphering_enabled_status,
                                         tdsnv_fake_security_enabled);
            
            /* Send RRC_SYNC_IND if Fake Security is enabled */
            if(tdsnv_fake_security_enabled)
            {
              /* For CS send the RRC_SYNC_IND for Integrity Protection */
              tdsrrcidt_send_sync_ind(RRC_INTEGRITY_ESTABLISHED,
                                   RRC_CS_DOMAIN_CN_ID);
            }
          }
          else
          {
            tdsrrcidt_cs_saved_open_session_req.open_sess_req_status = TRUE;
            switch(tdsrrc_get_state())
            {
              case TDSRRC_STATE_URA_PCH:
              case TDSRRC_STATE_CELL_PCH:   
                if(TDSRRCCU_COMPLETED == tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_IDT ) )
                {
                   tdsrrcidt_cs_saved_open_session_req.cu_init_by_idt = TRUE;
                   /* initiate cel update if not already in progress */
                   tdsrrcdt_send_cell_update_initiate_req(TDSRRC_UL_DATA_TRANSMISSION);
                }
                 
			    /* Wait until Cell update procedure is completed */
                tdsrrcidt_substate = TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;  
                return;
               
              default:
                break;
            }

            if(TDSRRCCU_NOT_COMPLETED == 
               tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_IDT ) )
            {
              /* Wait until Cell update procedure is completed */
              tdsrrcidt_substate = TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;  
              break;
            }

            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Re-transmit IDT for CS-Domain");
            tdsrrcidt_send_cs_initial_direct_transfer_msg();  
          }           
        }
        else
        {
          tdsrrcidt_delete_open_session_req(RRC_CS_DOMAIN_CN_ID);
        }
      }
      else if((tdsrrcidt_ps_saved_open_session_req.mui_status == TRUE) &&
              (tdsrrcidt_ps_saved_open_session_req.mui == 
               cmd_ptr->cmd.downlink_l2ack_ind.mui)
              )
      {
        /*if L2_ACK for SMC is not received before L2_ACK for IDT,
          send open_sess_cnf here, else open_session_cnf would have
          been sent by SMC just before sending SYNC_IND
         */
        if(tdsrrcidt_ps_saved_open_session_req.open_sess_cnf_required == TRUE)
        {
		  /* Check and send any blocked SCRR for this domain */
	      boolean ps_scrr_active = tdsrrcdt_check_and_send_pending_scrr(RRC_PS_DOMAIN_CN_ID);

          /* Send a cnf to NAS if L2 ACK succeeded.Also if there is a
		     pending SCRR for this domain,send a Cnf to NAS as we will
			 not be pursuing the retransmissions */
          if((SUCCESS == cmd_ptr->cmd.downlink_l2ack_ind.status)||
			 (TRUE == ps_scrr_active))
          {
            tdsrrcidt_send_open_session_cnf(RRC_OPEN_SESS_SUCCESS,
                                         RRC_PS_DOMAIN_CN_ID);

            tdsrrcidt_ps_saved_open_session_req.open_sess_cnf_required = FALSE;

            /* Delete the saved TDSRRC_OPEN_SESSION_REQ */
            tdsrrcidt_delete_open_session_req(RRC_PS_DOMAIN_CN_ID);

            TDSRRC_MSG3(MSG_LEGACY_HIGH,"Security NV settings: Integrity %d, Ciphering_status %d, Fake %d",
                                         tdsnv_integrity_enabled,
                                         tdsnv_ciphering_enabled_status,
                                         tdsnv_fake_security_enabled);
            
            /* Send RRC_SYNC_IND if Fake Security is enabled */
            if(tdsnv_fake_security_enabled)
            {
              /* For PS send the RRC_SYNC_IND for Integrity Protection */
              tdsrrcidt_send_sync_ind(RRC_INTEGRITY_ESTABLISHED,
                                   RRC_PS_DOMAIN_CN_ID);
            }
          }
          else
          {
             /* for all other states */
             tdsrrcidt_ps_saved_open_session_req.open_sess_req_status = TRUE;
             switch(tdsrrc_get_state())
             {
               case TDSRRC_STATE_URA_PCH:
               case TDSRRC_STATE_CELL_PCH:   

                 if(TDSRRCCU_COMPLETED == tdsrrccu_get_cell_update_complete_status
                    ( TDSRRC_PROCEDURE_IDT ) )
                 {
                   tdsrrcidt_ps_saved_open_session_req.cu_init_by_idt = TRUE;
                   tdsrrcdt_send_cell_update_initiate_req(TDSRRC_UL_DATA_TRANSMISSION);
                 }
                  
                 /* Wait until Cell update procedure is completed */
                 tdsrrcidt_substate = TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;  
                 return;

               default:
                 break;
             }

             if(TDSRRCCU_NOT_COMPLETED == 
                  tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_IDT ) )
             {
                /* Wait until Cell update procedure is completed */
                tdsrrcidt_substate = TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;  
                break;
             }
         
             TDSRRC_MSG0(MSG_LEGACY_HIGH,"Re-transmit IDT for PS-Domain");       
             tdsrrcidt_send_ps_initial_direct_transfer_msg();  
          }     
        }
        else
        {
           tdsrrcidt_delete_open_session_req(RRC_PS_DOMAIN_CN_ID);
        }

      }
      /* Reset the state machine */
      if((tdsrrcidt_cs_saved_open_session_req.mui_status == FALSE)  &&
         (tdsrrcidt_ps_saved_open_session_req.mui_status == FALSE) 
         )
      {
        tdsrrcidt_substate = TDSRRCIDT_INITIAL;  
      }
      break;

    case TDSRRC_OPEN_SESSION_REQ:
      if((TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_IDT ) )&&
        (tdsrrcidt_check_rrc_connection_status(cmd_ptr) == TRUE))
      {
        if(tdsis_ue_oos_and_em_call_attempted(cmd_ptr, TDSRRC_PROCEDURE_IDT))
        {
          tdsrrcidt_substate = TDSRRCIDT_INITIAL;  
          return;
        }
      }
      /* Before opening a new signalling session, be sure that there isn't one 
       * alreay open to the requested domain.  Also validate contents of the
       * open session request
       */
      if(TDSRRCDT_SUCCESS != tdsrrcidt_validate_open_session_req(cmd_ptr,&session_status))
      {
        if(session_status == RRC_OPEN_SESS_FAILURE_AC_BARD)
        {
          /* Access Class is bard for this domain,So send RRC_OPEN_SESS_CNF Primitive
             to MM with Failure status as RRC_OPEN_SESS_FAILURE_AC_BARD */
          tdsrrcidt_send_open_session_cnf(RRC_OPEN_SESS_FAILURE_AC_BARD,
                                    cmd_ptr->cmd.open_sess_req.cn_domain_id);
        }
        else
        {
          /* Send RRC_OPEN_SESS_CNF Primitive to MM with Failure status */
          tdsrrcidt_send_open_session_cnf(RRC_OPEN_SESS_FAILURE,
                                    cmd_ptr->cmd.open_sess_req.cn_domain_id);
        }
        return;
      }
    
      /* save the contents of the open_session_req message. will be used
       * later to send out the idt message
       */
      if(TDSRRCDT_SUCCESS != tdsrrcidt_save_open_session_req(cmd_ptr))
      {
        /* Send RRC_OPEN_SESS_CNF Primitive to MM with Failure status */
        tdsrrcidt_send_open_session_cnf(RRC_OPEN_SESS_FAILURE,
                                    cmd_ptr->cmd.open_sess_req.cn_domain_id);
        return;
      }

      if(tdsis_ue_oos_and_em_call_attempted(cmd_ptr, TDSRRC_PROCEDURE_IDT))
      {
        tdsrrcidt_substate = TDSRRCIDT_INITIAL;  
      }
      else
      {

        tdsrrcidt_set_est_cause(cmd_ptr->cmd.open_sess_req.est_cause, 
               TDSRRC_PROCEDURE_IDT,cmd_ptr->cmd.open_sess_req.cn_domain_id);
        if (cmd_ptr->cmd.open_sess_req.cn_domain_id == RRC_PS_DOMAIN_CN_ID)
        {
            /* Both PS and CS hasn't receive the RAB setup. */
            if ((mn_voice_call_vote_for_reselection(as_id) == NAS_VOTE_FOR_RESEL_FALSE)&&
                (sm_ps_call_vote_for_reselection(as_id) == NAS_VOTE_FOR_RESEL_FALSE)&&
                (TRUE == tdsrrc_hold_ps_tmr_expired))
            {
              if (tdsrrc_nv_hold_ps_tmr_value > 0)
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Hold PS: hold %d ms", tdsrrc_nv_hold_ps_tmr_value*10);
                tdsrrctmr_start_timer(TDSRRCTMR_HOLD_PS_TIMER, tdsrrc_nv_hold_ps_tmr_value*10);
                tdsrrc_hold_ps_tmr_expired = FALSE;
              }
              else
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Hold PS: don't hold PS");
              }
            }
        }

        /* Build and send the Initial Direct Transfer Msg to Send Chain */
        if(tdsrrcidt_cs_saved_open_session_req.open_sess_req_status == TRUE)
        {
          tdsrrcidt_send_cs_initial_direct_transfer_msg();
        }
        else if((tdsrrcidt_ps_saved_open_session_req.open_sess_req_status == TRUE)&&(TRUE == tdsrrc_hold_ps_tmr_expired))
        {
          tdsrrcidt_send_ps_initial_direct_transfer_msg();
        }
      }
      break;


    case TDSRRC_NAS_DATA_TRANSFER_IND:
      tdsrrcidt_send_nas_data_ind();  
      /* Reset the state machine */  
      tdsrrcidt_delete_open_session_req(RRC_CS_DOMAIN_CN_ID);  
      tdsrrcidt_delete_open_session_req(RRC_PS_DOMAIN_CN_ID);
      tdsrrcidt_reset_est_cause(TDSRRC_PROCEDURE_CU);  
      tdsrrcidt_substate = TDSRRCIDT_INITIAL; 
      break;


    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d for IDT in substate:%d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcidt_substate);
      break;
  }
 
} /* tdsrrcidt_process_idt_wait_for_l2ack_substate */

/*===========================================================================

FUNCTION tdsrrcidt_process_cell_fach_state

DESCRIPTION
  This function processes all events that are dispatched to Initial direct
  Transfer procedure in CELL_FACH state of Connected mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcidt_process_cell_fach_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  switch (tdsrrcidt_substate)
  {
    case TDSRRCIDT_INITIAL:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_OPEN_SESSION_REQ:

        case TDSRRC_NAS_DATA_TRANSFER_IND:
          tdsrrcidt_process_idt_initial_substate(cmd_ptr);
          break;
        case TDSRRC_HOLD_PS_TIMER_EXPIRED_IND:
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Hold PS:Handle pending timer out at ps open session status: %d at state %d",
                    tdsrrcidt_ps_saved_open_session_req.open_sess_req_status,tdsrrcidt_substate);
          tdsrrc_hold_ps_tmr_expired = TRUE;
          if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_IDT ))
          {
            tdsrrcidt_substate = TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;
          }
          else if(tdsrrcidt_ps_saved_open_session_req.open_sess_req_status == TRUE)
          {
            tdsrrcidt_send_ps_initial_direct_transfer_msg();
          }
          break;
        
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"IDT:Invalid eventin Substate: %d",tdsrrcidt_substate);
          break;
      }
      break;

    case TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF: 
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
        case TDSRRC_OPEN_SESSION_REQ:
        case TDSRRC_NAS_DATA_TRANSFER_IND:
          tdsrrcidt_process_idt_wait_for_cell_update_comp_cnf_substate(cmd_ptr);
          break;
        case TDSRRC_HOLD_PS_TIMER_EXPIRED_IND:
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Hold PS:Handle pending timer out at ps open session status: %d at state %d",
                    tdsrrcidt_ps_saved_open_session_req.open_sess_req_status,tdsrrcidt_substate);
          tdsrrc_hold_ps_tmr_expired = TRUE;
          if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_IDT ))
          {
            tdsrrcidt_substate = TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;
          }
          else 
          {
            tdsrrcidt_process_idt_wait_for_cell_update_comp_cnf_substate(cmd_ptr);
          }
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"IDT:Invalid event in Substate: %d",tdsrrcidt_substate);
          break;
      }
      break;

    case TDSRRCIDT_WAIT_FOR_L2ACK:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_DOWNLINK_L2ACK_IND:
        case TDSRRC_OPEN_SESSION_REQ:
        case TDSRRC_NAS_DATA_TRANSFER_IND:
          tdsrrcidt_process_idt_wait_for_l2ack_substate(cmd_ptr);
          break;
        case TDSRRC_HOLD_PS_TIMER_EXPIRED_IND:
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Hold PS:Handle pending timer out at ps open session status: %d at state %d",
                    tdsrrcidt_ps_saved_open_session_req.open_sess_req_status,tdsrrcidt_substate);
          tdsrrc_hold_ps_tmr_expired = TRUE;
          if(tdsrrcidt_ps_saved_open_session_req.open_sess_req_status == TRUE)
          {
            tdsrrcidt_send_ps_initial_direct_transfer_msg();
          }
          break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"IDT:Invalid event in Substate: %d",tdsrrcidt_substate);
          break;
      }
      break;


    default:           
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"IDT:Invalid substate:%d in CELL_FACH",tdsrrcidt_substate);
      break;
  }  
}/* tdsrrcidt_process_cell_fach_state */
/*===========================================================================

FUNCTION tdsrrcidt_process_cell_dch_state

DESCRIPTION
  This function processes all events that are dispatched to Initial direct
  Transfer procedure in CELL_DCH state of Connected mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcidt_process_cell_dch_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  switch (tdsrrcidt_substate)
  {
    case TDSRRCIDT_INITIAL:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_OPEN_SESSION_REQ:
        case TDSRRC_NAS_DATA_TRANSFER_IND:
          tdsrrcidt_process_idt_initial_substate(cmd_ptr);
          break;
        case TDSRRC_HOLD_PS_TIMER_EXPIRED_IND:
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Hold PS:Handle pending timer out at ps open session status: %d at state %d",
                    tdsrrcidt_ps_saved_open_session_req.open_sess_req_status,tdsrrcidt_substate);
          tdsrrc_hold_ps_tmr_expired = TRUE;
          if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_IDT ))
          {
            tdsrrcidt_substate = TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;
          }
          else if(tdsrrcidt_ps_saved_open_session_req.open_sess_req_status == TRUE)
          {
            tdsrrcidt_send_ps_initial_direct_transfer_msg();
          }
          break;
        
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"IDT:Invalid event in Substate: %d",tdsrrcidt_substate);
          break;
      }
      break;

    case TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF: 
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
        case TDSRRC_OPEN_SESSION_REQ:
        case TDSRRC_NAS_DATA_TRANSFER_IND:
          tdsrrcidt_process_idt_wait_for_cell_update_comp_cnf_substate(cmd_ptr);
          break;
        case TDSRRC_HOLD_PS_TIMER_EXPIRED_IND:
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Hold PS:Handle pending timer out at ps open session status: %d at state %d",
                    tdsrrcidt_ps_saved_open_session_req.open_sess_req_status,tdsrrcidt_substate);
          tdsrrc_hold_ps_tmr_expired = TRUE;
          if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_IDT ))
          {
            tdsrrcidt_substate = TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;
          }
          else 
          {
            tdsrrcidt_process_idt_wait_for_cell_update_comp_cnf_substate(cmd_ptr);
          }
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"IDT:Invalid event in Substate: %d",tdsrrcidt_substate);
          break;
      }
      break;

    case TDSRRCIDT_WAIT_FOR_L2ACK:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_DOWNLINK_L2ACK_IND:
        case TDSRRC_OPEN_SESSION_REQ:
        case TDSRRC_NAS_DATA_TRANSFER_IND:
          tdsrrcidt_process_idt_wait_for_l2ack_substate(cmd_ptr);
          break;
        case TDSRRC_HOLD_PS_TIMER_EXPIRED_IND:
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Hold PS:Handle pending timer out at ps open session status: %d at state %d",
                    tdsrrcidt_ps_saved_open_session_req.open_sess_req_status,tdsrrcidt_substate);
          tdsrrc_hold_ps_tmr_expired = TRUE;
          if(tdsrrcidt_ps_saved_open_session_req.open_sess_req_status == TRUE)
          {
            tdsrrcidt_send_ps_initial_direct_transfer_msg();
          }
          break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"IDT:Invalid event in Substate: %d",tdsrrcidt_substate);
          break;
      }
      break;

    default:           
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"IDT:Invalid substate:%d in CELL_DCH",tdsrrcidt_substate);
      break;
  }  
}/* tdsrrcidt_process_cell_dch_state */

/*===========================================================================

FUNCTION tdsrrcidt_process_cell_ura_pch_state

DESCRIPTION
  This function processes all events that are dispatched to Initial direct
  Transfer procedure in either in CELL_PCH or URA_PCH state of Connected mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcidt_process_cell_ura_pch_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  switch (tdsrrcidt_substate)
  {
    case TDSRRCIDT_INITIAL:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_OPEN_SESSION_REQ:
          tdsrrcidt_process_idt_initial_substate(cmd_ptr);
          break;
        case TDSRRC_HOLD_PS_TIMER_EXPIRED_IND: 
          tdsrrc_hold_ps_tmr_expired = TRUE;
          if ((TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_IDT )))
          {
            tdsrrcidt_substate = TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;
          }
          else 
          {
                /* change the owner of establishment cause to CU */
              tdsrrcidt_change_owner_est_cause(TDSRRC_PROCEDURE_CU);
              if(tdsrrcidt_is_idt_buffered(RRC_CS_DOMAIN_CN_ID) == TRUE)
              {
                tdsrrcidt_cs_saved_open_session_req.cu_init_by_idt = TRUE;
              }
              if(tdsrrcidt_is_idt_buffered(RRC_PS_DOMAIN_CN_ID) == TRUE)
              {
                tdsrrcidt_ps_saved_open_session_req.cu_init_by_idt = TRUE;
              }
              /* initiate cell update if it not already running */
              /* with cause uplink data transmission */
              tdsrrcdt_send_cell_update_initiate_req(TDSRRC_UL_DATA_TRANSMISSION);

          /* Wait until Cell update procedure is completed */
          tdsrrcidt_substate = TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;  
          }
          break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"IDT:Invalid event in Substate: %d",tdsrrcidt_substate);
          break;
      }
      break;

    case TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF:
      if (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_HOLD_PS_TIMER_EXPIRED_IND)
      {
        tdsrrc_hold_ps_tmr_expired = TRUE;
      }
      if (TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_IDT ))
      {
        tdsrrcidt_substate = TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;
      }
      else
      {
        tdsrrcidt_process_idt_wait_for_cell_update_comp_cnf_substate(cmd_ptr);
      }
      break;

    default:
      if (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_HOLD_PS_TIMER_EXPIRED_IND)
      {
        tdsrrc_hold_ps_tmr_expired = TRUE;
      }
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"IDT:Invalid substate:%d in PCH State: %d",tdsrrcidt_substate);
      break;
  }  
}/* tdsrrcidt_process_cell_ura_pch_state */
/*===========================================================================

FUNCTION tdsrrcidt_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the ininitial direct transfer
  procedure.
  The following events are handled by this procedure:

    TDSRRC_OPEN_SESSION_REQ Primitive (received from MM)
    TDSRRC_CELL_UPDATE_COMPLETE_CNF primitive (received from CELL UPDATE Procedure)
    TDSRRC_NAS_DATA_TRANSFER_IND primitive from RR
    TDSRRC_DOWNLINK_L2ACK_IND primitive (received from RLC)

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcidt_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
)
{
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();
#ifdef FEATURE_TDSCDMA_SRNS_MO_CALL_DEFER  
  mm_cmd_type *mm_buf_ptr = NULL; /*Added for SRNS reallocation send rrc_close_session_ind*/
            /*Added for SRNS reallocation
              when receive tds rrc open session req for cs call setup and rrc is in srns reallocation process and found LAC/RAC changed
              send close_session_ind to NAS for silent redial */
  if((TDSRRC_OPEN_SESSION_REQ == cmd_ptr->cmd_hdr.cmd_id)&&
    (TRUE == tdsrrcrb_is_srns_in_progress()) && (TRUE == tdsrrc_rbrc.srns_reallocation_trigger)
  	&&(RRC_EST_ORIG_CONV_CALL == cmd_ptr->cmd.open_sess_req.est_cause))
  {
      if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_CLOSE_SESSION_IND)) == NULL) 
      {
          ERR_FATAL("No MM buffer for CSP to MM cmd 0x%x",RRC_CLOSE_SESSION_IND,0,0);
      }
      else
      {
            #ifdef FEATURE_DUAL_SIM
          mm_buf_ptr->cmd.rrc_close_session_ind.as_id = tdsrrc_get_as_id();
            #else
          mm_buf_ptr->cmd.rrc_close_session_ind.as_id = SYS_MODEM_AS_ID_1;
          #endif
          mm_buf_ptr->cmd.rrc_close_session_ind.cn_domain_id = RRC_CS_DOMAIN_CN_ID;
             /* Put the command on MM queue */
            if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
          {
              tdscfa_mm_put_cmd(mm_buf_ptr);
          }
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"FEATURE_TDSCDMA_SRNS_MO_CALL_DEFER: Send close session ind to MM directly for LAC change SRNS reallocation");	 
          mm_put_cmd(mm_buf_ptr);
     }
  }
  else if(TDSRRC_STATE_CHANGE_IND == cmd_ptr->cmd_hdr.cmd_id)
#else
    if(TDSRRC_STATE_CHANGE_IND == cmd_ptr->cmd_hdr.cmd_id)
#endif
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"State-Change-Ind idt-state %d in rrc-state %d",tdsrrcidt_substate,rrc_state);
    tdsrrctmr_stop_timer(TDSRRCTMR_HOLD_PS_TIMER);
    tdsrrc_hold_ps_tmr_expired = TRUE;
    
    //Any pending message memory needs to be de-allocated here 
    tdsrrcidt_delete_open_session_req(RRC_CS_DOMAIN_CN_ID);  
    tdsrrcidt_delete_open_session_req(RRC_PS_DOMAIN_CN_ID);  
    tdsrrcidt_substate = TDSRRCIDT_INITIAL;  
  }
  else
  {
    switch (rrc_state)
    {
      case TDSRRC_STATE_CELL_FACH:    
      tdsrrcidt_process_cell_fach_state(cmd_ptr); 
      break;
  
      case TDSRRC_STATE_CELL_DCH:  
      tdsrrcidt_process_cell_dch_state(cmd_ptr); 
      break;
  
      case TDSRRC_STATE_CELL_PCH:
      case TDSRRC_STATE_URA_PCH:
      tdsrrcidt_process_cell_ura_pch_state(cmd_ptr);
      break;
  
      default:  
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"IDT:Event forwarded in invalid State: %d",rrc_state);
      break;
    }  
  }
} /* tdsrrcidt_procedure_event_handler */
/*===========================================================================

FUNCTION tdsrrcdt_is_cs_domain_open

DESCRIPTION
  This function returns whether session for CS domain is open or not.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - CS Domain is Open.
  FALSE - CS Domain isn't Open.

===========================================================================*/
boolean tdsrrcdt_is_cs_domain_open(void)
{
  return tdsrrcdt_int_data.cs_domain_open;
}
/*===========================================================================

FUNCTION tdsrrcdt_is_ps_domain_open

DESCRIPTION
  This function returns whether session for PS domain is open or not.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - PS Domain is Open.
  FALSE - PS Domain isn't Open.

===========================================================================*/
boolean tdsrrcdt_is_ps_domain_open(void)
{
  return tdsrrcdt_int_data.ps_domain_open;
}

/*===========================================================================

FUNCTION tdsrrcudt_get_cn_domain_for_mui

DESCRIPTION
  This function returns the CN Domain Id for the mui

DEPENDENCIES
  None.

RETURN VALUE
  CN domain id

===========================================================================*/
static rrc_cn_domain_identity_e_type tdsrrcudt_get_cn_domain_for_mui
(
  uint8 mui              
)
{
  /* Initialize domain id to keep lint happy */
  rrc_cn_domain_identity_e_type domain_id = RRC_CS_DOMAIN_CN_ID;
  boolean domain_found = FALSE;
  uint32 count = 0;

  for (count = 0;count < MAX_NAS_MESSAGES_FOR_CS_DOMAIN;count++ )
  {
    if((tdsrrcudt_cs_saved_data[count].mui == mui) && 
       (tdsrrcudt_cs_saved_data[count].mui_status == TRUE)) 
    {
      domain_id = RRC_CS_DOMAIN_CN_ID;
	    domain_found = TRUE;
      break;
    }
  }

  if(domain_found != TRUE)
  {
    for (count = 0;count < TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN;count++ )
    {
      if((tdsrrcudt_ps_saved_data[count].mui == mui) && 
         (tdsrrcudt_ps_saved_data[count].mui_status == TRUE)) 
      {
        domain_id = RRC_PS_DOMAIN_CN_ID;
        domain_found = TRUE;
        break;
      }
    }
  }
  return domain_id;
}

/*===========================================================================

FUNCTION tdsrrcudt_validate_data_req  

DESCRIPTION
  This function validates TDSRRC_DATA_REQ command received from MM task.

DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcdt_status_e_type: It returns TDSRRCDT_SUCCESS if it is successfully validates 
  the TDSRRC_OPEN_SESSION_REQ command. Otherwise it returns TDSRRCDT_FAILURE_MINOR.

===========================================================================*/

static tdsrrcdt_status_e_type tdsrrcudt_validate_data_req
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{
  tdsrrcdt_status_e_type status = TDSRRCDT_FAILURE_MINOR; 

  boolean domain_open;                  /* Indicates if the domain is open */

   /* Check whether signalling connection is open for the received CN
    * Domain Id.
    */
  domain_open = FALSE;

  switch (cmd_ptr->cmd.data_req.cn_domain_id)
  {
    case RRC_CS_DOMAIN_CN_ID :
      if (tdsrrcdt_int_data.cs_domain_open )
      {
        domain_open = TRUE;
      }
      break;

    case RRC_PS_DOMAIN_CN_ID :
      if (tdsrrcdt_int_data.ps_domain_open)
      {
        domain_open = TRUE;
      }
      break;

    default :
      /* MM sent us an invalid CN Domain ID
       */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid CN Domain ID %d",cmd_ptr->cmd.data_req.cn_domain_id);
      break;
  }

  /* If the domain is not open, print a message and exit
   */
  if (domain_open == FALSE)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"CN Domain ID not open: %d",cmd_ptr->cmd.data_req.cn_domain_id);
    
  }
  else
  {

    /* Validate the Data Req primitive. Check to make sure that the payload 
     * isn't too large or isn't empty.
     */
    if ((cmd_ptr->cmd.data_req.nas_msg.length > TDSRRCDT_MAX_PAYLOAD) ||
        (cmd_ptr->cmd.data_req.nas_msg.length == 0))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid payload size: %d",cmd_ptr->cmd.data_req.nas_msg.length);
    
    }
    else
    {
      status = TDSRRCDT_SUCCESS;
    }
  }
  return (status);

} /* tdsrrcudt_validate_data_req */
/*===========================================================================

FUNCTION tdsrrcudt_any_saved_data_req_for_cn_domain

DESCRIPTION
  This function will give status of any saved data req for a given CN domain

DEPENDENCIES
  None.

RETURN VALUE
  TRUE:- If any dat req saved of in process for a CN domain
  FALSE:- otherwise

===========================================================================*/
boolean tdsrrcudt_any_saved_data_req_for_cn_domain
(
  rrc_cn_domain_identity_e_type cn_domain_id
)
{
  uint8 count;                          /* Local variable for index */
  boolean status = FALSE;
  switch (cn_domain_id)
  {
    case RRC_CS_DOMAIN_CN_ID :
      for (count = 0; count <MAX_NAS_MESSAGES_FOR_CS_DOMAIN; count++ )
      {
        if(tdsrrcudt_cs_saved_data[count].mui_status || 
           tdsrrcudt_cs_saved_data[count].data_req_status)
        {
          status = TRUE;
          break;
        }
      }
      break;
    
    case RRC_PS_DOMAIN_CN_ID :
      for (count = 0; count <TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN; count++ )
      {
        if(tdsrrcudt_ps_saved_data[count].mui_status || 
           tdsrrcudt_ps_saved_data[count].data_req_status)
        {
          status = TRUE;
          break;
        }
      }
      break;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid CN Domain:%d",cn_domain_id);
      break;
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Saved data req for CN domain %d,status = %d",cn_domain_id,status);
  return status;
} /* tdsrrcudt_delete_all_saved_data_req */

/*===========================================================================

FUNCTION tdsrrcudt_delete_all_saved_data_req

DESCRIPTION
  This function deletes the Data Req cmd from the static variable
  for the specified CN domain

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcudt_delete_all_saved_data_req
(
  rrc_cn_domain_identity_e_type cn_domain_id
)
{
  uint8 count;                          /* Local variable for index */
  switch (cn_domain_id)
  {
    case RRC_CS_DOMAIN_CN_ID :
      for (count = 0; count <MAX_NAS_MESSAGES_FOR_CS_DOMAIN; count++ )
      {
        if(tdsrrcudt_cs_saved_data[count].mui_status || 
           tdsrrcudt_cs_saved_data[count].data_req_status)
        {
          tdsrrcudt_cs_saved_data[count].mui_status = FALSE;
          tdsrrcudt_cs_saved_data[count].data_req_status = FALSE;

          if(tdsrrcudt_cs_saved_data[count].data_req.nas_msg.value != NULL)
          {
            tdsrrc_free(tdsrrcudt_cs_saved_data[count].data_req.nas_msg.value);
            tdsrrcudt_cs_saved_data[count].data_req.nas_msg.value = NULL;
            tdsrrcudt_cs_saved_data[count].data_req.nas_msg.length = 0;
          }
        }
      }
      break;
    
    case RRC_PS_DOMAIN_CN_ID :
      for (count = 0; count <TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN; count++ )
      {
        if(tdsrrcudt_ps_saved_data[count].mui_status || 
           tdsrrcudt_ps_saved_data[count].data_req_status)
        {
          tdsrrcudt_ps_saved_data[count].mui_status = FALSE;
          tdsrrcudt_ps_saved_data[count].data_req_status = FALSE;

          if(tdsrrcudt_ps_saved_data[count].data_req.nas_msg.value != NULL)
          {
            tdsrrc_free(tdsrrcudt_ps_saved_data[count].data_req.nas_msg.value);
            tdsrrcudt_ps_saved_data[count].data_req.nas_msg.value = NULL;
            tdsrrcudt_ps_saved_data[count].data_req.nas_msg.length = 0;
          }
        }
      }
      break;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid CN Domain:%d",cn_domain_id);
      break;
  }
} /* tdsrrcudt_delete_all_saved_data_req */
/*===========================================================================

FUNCTION tdsrrcudt_delete_saved_data_req

DESCRIPTION
  This function deletes the Data Req cmd from the static variable
  for the specified MUI of a particular Radio Bearer

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
static uecomdef_status_e_type tdsrrcudt_delete_saved_data_req
(
  rrc_nas_msg_priority_e_type priority,
                                        /* Priority selected NAS for 
                                           xmission */
  uint8 rcvd_mui                        /* Received MUI from RLC */
)
{
  uint8 count;                          /* Local variable for index */
  uecomdef_status_e_type status = FAILURE;
  

  for (count = 0; count < MAX_NAS_MESSAGES_FOR_CS_DOMAIN; count++ )
  {
    if(tdsrrcudt_cs_saved_data[count].mui_status == TRUE)
    {
      if( (tdsrrcudt_cs_saved_data[count].mui == rcvd_mui) &&
          (tdsrrcudt_cs_saved_data[count].data_req.priority == priority) )
      {
        tdsrrcudt_cs_saved_data[count].mui_status = FALSE;
        tdsrrcudt_cs_saved_data[count].data_req_status = FALSE;
        if (tdsrrcudt_cs_saved_data[count].data_req.nas_msg.value !=
            NULL)
        {
          tdsrrc_free(tdsrrcudt_cs_saved_data[count].data_req.nas_msg.value);
          tdsrrcudt_cs_saved_data[count].data_req.nas_msg.value = NULL;
          tdsrrcudt_cs_saved_data[count].data_req.nas_msg.length = 0;
        }
        status = SUCCESS;
        break;
      }
      
    }
  }
  if(status == SUCCESS)
  {
    return(status);
  }
  for (count = 0; count < TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN; count++ )
  {
    if(tdsrrcudt_ps_saved_data[count].mui_status == TRUE)
    {
      if( (tdsrrcudt_ps_saved_data[count].mui == rcvd_mui) &&
          (tdsrrcudt_ps_saved_data[count].data_req.priority == priority) )
      {
        tdsrrcudt_ps_saved_data[count].mui_status = FALSE;
        tdsrrcudt_ps_saved_data[count].data_req_status = FALSE;
        if (tdsrrcudt_ps_saved_data[count].data_req.nas_msg.value !=
            NULL)
        {
          tdsrrc_free(tdsrrcudt_ps_saved_data[count].data_req.nas_msg.value);
          tdsrrcudt_ps_saved_data[count].data_req.nas_msg.value = NULL;
          tdsrrcudt_ps_saved_data[count].data_req.nas_msg.length = 0;
        }
        
        status = SUCCESS;
        break;
      }
      
    }
  }
  return (status); 
} /* tdsrrcudt_delete_saved_data_req */
/*===========================================================================

FUNCTION tdsrrcudt_check_if_msg_status_can_be_saved

DESCRIPTION
  This function checks the saved message status and verifies whether new can
  be saved or not.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
static uecomdef_status_e_type tdsrrcudt_check_if_msg_status_can_be_saved
(
  rrc_cn_domain_identity_e_type cn_domain_id,
  uint8 *index
)
{
  uecomdef_status_e_type status = FAILURE;
  uint8 count;                /* Local variable for indexing */

  switch (cn_domain_id)
  {
    case RRC_CS_DOMAIN_CN_ID :
      for (count = 0; count <MAX_NAS_MESSAGES_FOR_CS_DOMAIN; count++ )
      {
        if((tdsrrcudt_cs_saved_data[count].mui_status == FALSE) &&
           (tdsrrcudt_cs_saved_data[count].data_req_status ==FALSE) )
        {
          status = SUCCESS;
          *index = count;
          break;
        }
      }
      break;
    
    case RRC_PS_DOMAIN_CN_ID :
      for (count = 0; count <TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN; count++ )
      {
        if((tdsrrcudt_ps_saved_data[count].mui_status == FALSE) &&
           (tdsrrcudt_ps_saved_data[count].data_req_status == FALSE) )
        {
          status = SUCCESS;
          *index = count;
          break;
        }
      }
      break;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid CN domain:%d",cn_domain_id);
      break;
  }
  return (status);
} /* tdsrrcudt_check_if_msg_status_can_be_saved */
/*===========================================================================

FUNCTION tdsrrcudt_check_saved_message_status

DESCRIPTION
  This function checks the saved message status and verifies whether new can
  be saved or not.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
static uecomdef_status_e_type tdsrrcudt_check_saved_message_status
(
  void
)
{
  uecomdef_status_e_type status = FAILURE;
  uint8 count;                /* Local variable for indexing */

  for (count = 0; count <MAX_NAS_MESSAGES_FOR_CS_DOMAIN; count++ )
  {
    if((tdsrrcudt_cs_saved_data[count].mui_status == TRUE) ||
       (tdsrrcudt_cs_saved_data[count].data_req_status == TRUE) )
    {
      status = SUCCESS;
      break;
    }
  }
  if(status == SUCCESS)
  { 
    return(status);
  }

  for (count = 0; count <TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN; count++ )
  {
    if((tdsrrcudt_ps_saved_data[count].mui_status == TRUE) ||
       (tdsrrcudt_ps_saved_data[count].data_req_status == TRUE) )
    {
      status = SUCCESS;
      break;
    }
  }
 
  return (status);
} /* tdsrrcudt_check_saved_message_status */

/*===========================================================================

FUNCTION tdsrrcudt_check_saved_mui_status

DESCRIPTION
  This function checks the status of MUIs.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
static uecomdef_status_e_type tdsrrcudt_check_saved_mui_status
(
 void
)
{
  uecomdef_status_e_type status = FAILURE;
  uint8 count;                /* Local variable for indexing */

  
  for (count = 0; count <MAX_NAS_MESSAGES_FOR_CS_DOMAIN; count++ )
  {
    if(tdsrrcudt_cs_saved_data[count].mui_status == TRUE)
    {
      status = SUCCESS;
      break;
    }
  }

  if(status == SUCCESS)
  {
    return(status);
  }
  for (count = 0; count <TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN; count++ )
  {
    if(tdsrrcudt_ps_saved_data[count].mui_status == TRUE)
    {
      status = SUCCESS;
      break;
    }
  }
  return (status);
} /* tdsrrcudt_check_saved_mui_status */

/*===========================================================================

FUNCTION tdsrrcudt_save_data_req

DESCRIPTION
  This function saves the Data Req cmd in the static variable
  saved_data_req for CS/PS domain

DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcdt_status_e_type.

===========================================================================*/
static tdsrrcdt_status_e_type tdsrrcudt_save_data_req
(
  tdsrrc_cmd_type *cmd_ptr                             /* Pointer to RRC Command */
)
{
  unsigned char       *payload;                     /* Pointer to the MM payload */
  rrc_data_req_type   *save_temp_ptr=NULL;          /* pointer to save data req type */
  boolean             *save_data_req_status_ptr=NULL; /* data req flag pointer */
  uint8               index;                        /* Local variable for index */
  tdsrrcdt_status_e_type save_status = TDSRRCDT_FAILURE_MINOR;  /* save status indicator */

  if(tdsrrcudt_check_if_msg_status_can_be_saved(
       cmd_ptr->cmd.data_req.cn_domain_id, &index ) == SUCCESS)
  {
    switch (cmd_ptr->cmd.data_req.cn_domain_id)
    {
      case RRC_CS_DOMAIN_CN_ID :
  
        save_temp_ptr          = &tdsrrcudt_cs_saved_data[index].data_req;
        save_data_req_status_ptr = &tdsrrcudt_cs_saved_data[index].data_req_status;
        save_temp_ptr->cn_domain_id = RRC_CS_DOMAIN_CN_ID;
        break;
      
      case RRC_PS_DOMAIN_CN_ID :
        save_temp_ptr          = &tdsrrcudt_ps_saved_data[index].data_req;
        save_data_req_status_ptr = &tdsrrcudt_ps_saved_data[index].data_req_status;
        save_temp_ptr->cn_domain_id = RRC_PS_DOMAIN_CN_ID;
        break;
      
      default :
        ERR_FATAL("Invalid Domain ID %d",cmd_ptr->cmd.data_req.cn_domain_id,0,0);
        break;
    }
    /* Determine whether the Uplink Direct Transfer Message should go out on the
     * high or low priority radio bearer
     */
    if (cmd_ptr->cmd.data_req.priority == RRC_NAS_MSG_HIGH_PRIO)
    {
      save_temp_ptr->priority = RRC_NAS_MSG_HIGH_PRIO;
    }
    else
    {
      save_temp_ptr->priority = RRC_NAS_MSG_LOW_PRIO;
    }
#ifdef FEATURE_TDSCDMA_MTPL_THERMAL_BACKOFF 
    save_temp_ptr->is_emergency_call = cmd_ptr->cmd.data_req.is_emergency_call;
#endif					
    /* Allocate a buffer for the payload. This memory is released by 
     * tdsrrcsend_send_ul_sdu().
     */
    payload = tdsrrc_malloc(cmd_ptr->cmd.data_req.nas_msg.length);
    
    if (payload == NULL)
    {
      ERR_FATAL("malloc(%d) failure",cmd_ptr->cmd.data_req.nas_msg.length,0,0);
    }
    
    /* Copy the payload from the MM primitive into the newly allocated memory
     */
    memscpy(payload,
      cmd_ptr->cmd.data_req.nas_msg.length,
      cmd_ptr->cmd.data_req.nas_msg.value,
      cmd_ptr->cmd.data_req.nas_msg.length);
     /*Save Protocol discriminator & Transaction I.D from NAS OTA message*/
    /* Set the pointer to the payload and the length of the payload in the 
     * Initial Direct Transfer Message
     */
    save_temp_ptr->nas_msg.value = payload;
    save_temp_ptr->nas_msg.length = 
      (unsigned short) cmd_ptr->cmd.data_req.nas_msg.length;
    
    /* set the flag to indicate that DATA_REQ cmd contents are saved and 
     * return success status
     */
    *(save_data_req_status_ptr) = TRUE;
  
    save_status=TDSRRCDT_SUCCESS;
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Can't processData Req for Domain:%d",cmd_ptr->cmd.data_req.cn_domain_id);
  }
  return(save_status);
} /* tdsrrcudt_save_data_req */
/*===========================================================================

FUNCTION tdsrrcudt_send_uplink_direct_transfer_msg

DESCRIPTION
  This function sends the uplink direct transfer on the uplink DCCH.

DEPENDENCIES
  None.

RETURN VALUE
  void.

===========================================================================*/
static void tdsrrcudt_send_uplink_direct_transfer_msg
(
 void
)
{
  uint8 count_domain;                   /* Keep count of CS/PS*/
  uint8 count_index;                    /* Count for saved messages */
  tdsrrc_UL_DCCH_Message *msg_ptr;         /* Pointer to uplink DCCH message */ 
  
  rlc_lc_id_type ul_lc_id;              /* Uplink logical channel ID */

  tdsrrcsend_status_e_type send_status;    /* send status of UL message */

  uint8 mui;                            /* Local varibale for MUI */

  rrc_data_req_type *save_temp_ptr=NULL;
  boolean           *save_data_req_status_ptr=NULL;
  uint8             no_of_messages_for_domain;
  boolean         tmp_data_req_status = FALSE; /*Record whether we have message hasn't sent out. */
  boolean         tmp_mui_status = FALSE;  /* Record whether we have message hasn't got the ACK. */
  
  for (count_domain = 0; count_domain < RRC_MAX_CN_DOMAINS; count_domain++)  
  /* need count_domian 0,1 for cs and ps domain*/
  {
    if(count_domain == 0)
    {
      no_of_messages_for_domain = MAX_NAS_MESSAGES_FOR_CS_DOMAIN;
    }
    else
    {
      no_of_messages_for_domain = TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN;
      if (tdsrrc_hold_ps_tmr_expired == FALSE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Hold PS: hold PS timer is running");
      }
    }
    for ( count_index = 0; count_index < no_of_messages_for_domain; 
          count_index++)
    {
      if( count_domain == 0)
      {
        save_data_req_status_ptr = 
          &tdsrrcudt_cs_saved_data[count_index].data_req_status;
        save_temp_ptr = &tdsrrcudt_cs_saved_data[count_index].data_req;
        if (FALSE == tmp_data_req_status)
        {
          tmp_data_req_status = *save_data_req_status_ptr;
        }
        if (FALSE == tmp_mui_status)
        {
          tmp_mui_status = tdsrrcudt_cs_saved_data[count_index].mui_status;
        }
      }
      else 
      { 
        if(count_index < TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN )
        {
          save_data_req_status_ptr = 
            &tdsrrcudt_ps_saved_data[count_index].data_req_status;
          save_temp_ptr = &tdsrrcudt_ps_saved_data[count_index].data_req;
          
        }
        else
        {
           TDSRRC_MSG1(MSG_LEGACY_ERROR,"count_index %d cant be more than TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN",count_index);
           return;
        }
        
        if (FALSE == tmp_data_req_status)
        {
          tmp_data_req_status = *save_data_req_status_ptr;
        }
        if (FALSE == tmp_mui_status)
        {
          tmp_mui_status = tdsrrcudt_ps_saved_data[count_index].mui_status;
        }
      }
      if( (*(save_data_req_status_ptr) == TRUE)&&(((save_temp_ptr->cn_domain_id == RRC_PS_DOMAIN_CN_ID)
        &&(TRUE == tdsrrc_hold_ps_tmr_expired))||(save_temp_ptr->cn_domain_id == RRC_CS_DOMAIN_CN_ID)))
      {
        if((tdsis_scrr_in_progress) && (tdsrrcscrr_substate != TDSRRCSCRR_INITIAL))
        {
          if((save_temp_ptr->cn_domain_id == RRC_CS_DOMAIN_CN_ID) &&
             (tdsrrcscrr_saved_cs_close_session_req.mui_status != TDSRRCSCRR_MUI_STATUS_FALSE))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"SCRR for CS in progress. Discard CS UDT");
            return;
          }
          else if((save_temp_ptr->cn_domain_id == RRC_PS_DOMAIN_CN_ID) &&
             (tdsrrcscrr_saved_ps_close_session_req.mui_status != TDSRRCSCRR_MUI_STATUS_FALSE))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"SCRR for PS in progress. Discard PS UDT");
            return;
          }
        }

        /* Determine the logical channel id for the Uplink DCCH */
        if(save_temp_ptr->priority == RRC_NAS_MSG_HIGH_PRIO)
        {
           ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH,
                                             DCCH_DT_HIGH_PRI_RADIO_BEARER_ID, 
                                             UE_MODE_ACKNOWLEDGED_DATA);
        }
        else
        {
           ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH,
                                             DCCH_DT_LOW_PRI_RADIO_BEARER_ID, 
                                             UE_MODE_ACKNOWLEDGED_DATA);
        }
        /*lint -save -e650 */
        if( (ul_lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND) &&
            (save_temp_ptr->priority == RRC_NAS_MSG_LOW_PRIO) ) 
        {
        /* The logical channel id was not found. If we were looking for the low
         * priority radio bearer (RB 4), try the high priority radio bearer (RB 3).
         */
          
          ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH,
                                               DCCH_DT_HIGH_PRI_RADIO_BEARER_ID, 
                                               UE_MODE_ACKNOWLEDGED_DATA);

          save_temp_ptr->priority = RRC_NAS_MSG_HIGH_PRIO;
        }
        /*lint -restore */
        if (ul_lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RBs 3 and 4 not found by LCM");
  
          /*delete CS and PS domain saved info as LC ID is not present    */
          tdsrrcudt_delete_all_saved_data_req(RRC_CS_DOMAIN_CN_ID);
          tdsrrcudt_delete_all_saved_data_req(RRC_PS_DOMAIN_CN_ID);

          /* Reset the state machine */
          tdsrrcudt_substate = TDSRRCUDT_INITIAL;
  
          return;
        }
        else
        {
        /* Build an Uplink Direct Transfer Message. Start by allocating an Uplink 
         * DCCH message. This memory is released by tdsrrcsend_send_ul_sdu().
         */
          msg_ptr = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc( 
            sizeof(struct tdsrrc_UL_DCCH_Message));
          
         /* Call error fatal if the memory allocation failed
          */
          if (msg_ptr == NULL)
          {
            ERR_FATAL("malloc(%d) failure",sizeof(struct tdsrrc_UL_DCCH_Message),0,0);
          }    
          
         /* Identify this message as an Uplink Direct Transfer Message
          */
          TDSRRCSET_MSG_TYPE(msg_ptr->message,
              T_tdsrrc_UL_DCCH_MessageType_uplinkDirectTransfer);
          
        
          TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.uplinkDirectTransfer);

          if(tdsrrc_get_state() ==  TDSRRC_STATE_CELL_FACH)
          {
            /* Function call to Measurement Control & Report procedure. */
            if(TDSRRCMEAS_RACH_SUCCESSFUL == tdsrrcmeas_append_meas_rpt_on_rach(
               &msg_ptr->message.u.uplinkDirectTransfer.measuredResultsOnRACH, TDSRRC_PROCEDURE_UDT, FALSE))
            {
            /* if Measurement Control & Reportprocedure returns RRC_MEAS_RACH_SUCCESFUL,
               then enable the  bit_mask field */
              TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.uplinkDirectTransfer,
                rrc_UplinkDirectTransfer_,
                measuredResultsOnRACH);
             
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_MED,"RACH meas is not appended");
            }
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
            if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
            {
              if(SUCCESS == tdsrrcifreq_append_meas_rpt_on_rach(&(msg_ptr->message.u.uplinkDirectTransfer.laterNonCriticalExtensions.v690NonCriticalExtensions.
              uplinkDirectTransfer_v690ext.measuredResultsOnRACHinterFreq),TDSRRC_PROCEDURE_UDT))
              {
                if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR((&msg_ptr->message.u.uplinkDirectTransfer),
                    rrc_UplinkDirectTransfer_,
                    laterNonCriticalExtensions)))
                {
                  TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.uplinkDirectTransfer,
                    rrc_UplinkDirectTransfer_,
                    laterNonCriticalExtensions);
                  TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.uplinkDirectTransfer.laterNonCriticalExtensions);
                }
                if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR((&msg_ptr->message.u.uplinkDirectTransfer.laterNonCriticalExtensions),
                    rrc_UplinkDirectTransfer_laterNonCriticalExtensions_,
                    v690NonCriticalExtensions)))
                {
                  TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.uplinkDirectTransfer.laterNonCriticalExtensions,
                   rrc_UplinkDirectTransfer_laterNonCriticalExtensions_,
                   v690NonCriticalExtensions);
                  TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.uplinkDirectTransfer.laterNonCriticalExtensions.v690NonCriticalExtensions);
                  TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.uplinkDirectTransfer.laterNonCriticalExtensions.v690NonCriticalExtensions.uplinkDirectTransfer_v690ext);
                }
                TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.uplinkDirectTransfer.laterNonCriticalExtensions.v690NonCriticalExtensions.uplinkDirectTransfer_v690ext, 
                    rrc_UplinkDirectTransfer_v690ext_IEs_,
                    measuredResultsOnRACHinterFreq);
              }
            }
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/

          }
          

          /* Copy the contents of Saved TDSRRC_DATA_REQ 
          */    
          memscpy(msg_ptr->message.u.uplinkDirectTransfer.nas_Message.data,
            save_temp_ptr->nas_msg.length,
            save_temp_ptr->nas_msg.value, 
            save_temp_ptr->nas_msg.length);
      
          msg_ptr->message.u.uplinkDirectTransfer.nas_Message.numocts = 
            save_temp_ptr->nas_msg.length;
          
         /* Fill in the CN Domain id given by the command 
          */
    
          if(save_temp_ptr->cn_domain_id == RRC_CS_DOMAIN_CN_ID)
          {
            msg_ptr->message.u.uplinkDirectTransfer.cn_DomainIdentity = 
            tdsrrc_CN_DomainIdentity_cs_domain;
          }
          else
          {
            msg_ptr->message.u.uplinkDirectTransfer.cn_DomainIdentity =
              tdsrrc_CN_DomainIdentity_ps_domain;
          }
          
         /* Send the Uplink Direct Transfer Message on the Uplink DCCH
          */
          send_status = tdsrrcsend_send_ul_sdu_with_mui(TDSRRC_PROCEDURE_UDT,
                                            msg_ptr,
                                            ul_lc_id,
                                            TDSRRCSEND_L2ACK_REQUIRED,
                                            &mui
                                            );
  
          if (send_status == TDSRRCSEND_SUCCESS)
          {
            switch(save_temp_ptr->cn_domain_id)
            {
              case RRC_CS_DOMAIN_CN_ID :
                if(count_index < MAX_NAS_MESSAGES_FOR_CS_DOMAIN)
                {
                  tdsrrcudt_cs_saved_data[count_index].mui_status = TRUE;
                  tdsrrcudt_cs_saved_data[count_index].mui = mui;
#ifdef FEATURE_TDSCDMA_MTPL_THERMAL_BACKOFF 
                  if(TRUE == save_temp_ptr->is_emergency_call)
                  {
                    tdsrrcrce_set_rrc_est_cause_for_emergency_call();
                  }
#endif
                }
                break;
              
              case RRC_PS_DOMAIN_CN_ID :
                if(count_index < TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN)
                {
                  tdsrrcudt_ps_saved_data[count_index].mui_status = TRUE;
                  tdsrrcudt_ps_saved_data[count_index].mui = mui;
                }
                break;
              
              default :
                ERR_FATAL("Invalid Domain ID %d",save_temp_ptr->cn_domain_id,0,0);
                break;
            }
  
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sent Uplink Direct Transfer Message");  
            tdsrrcudt_substate = TDSRRCUDT_WAIT_FOR_L2ACK;
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Error %d sending Uplink Direct Xfer Msg",send_status);
           
            tdsrrcudt_delete_all_saved_data_req(RRC_CS_DOMAIN_CN_ID);
            tdsrrcudt_delete_all_saved_data_req(RRC_PS_DOMAIN_CN_ID);
          
            tdsrrcudt_substate = TDSRRCUDT_INITIAL;
  
          }
          /*set the flag to false.  no need to call delete_data_req as nas_msg.value is
           *already deleted in tdsrrcsend_send_ul_sdu
           */
          *(save_data_req_status_ptr) = FALSE;
        }            
      }
    } /* end for all messages */
  } /* end for CN domains */
  
  /* if RRC has no message need to send, but has message to wait for  l2 ACK, set to wait for l2 ack state.*/
  if (( TDSRRCUDT_WAIT_FOR_CELL_UPDATE_COMP_CNF == tdsrrcudt_substate)&&(TRUE == tmp_mui_status)&&(FALSE == tmp_data_req_status))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Has message wait L2ACK at cell update complete state");  
    tdsrrcudt_substate = TDSRRCUDT_WAIT_FOR_L2ACK;
  }
    /* if RRC has no message need to send, and has no  message to wait for  l2 ACK, set to initial state..*/
  else if(( TDSRRCUDT_WAIT_FOR_CELL_UPDATE_COMP_CNF == tdsrrcudt_substate) && (FALSE == tmp_data_req_status))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"No message wait L2ACK and no message send at cell update complete state");  
    tdsrrcudt_substate = TDSRRCUDT_INITIAL;
  }
}/* tdsrrcudt_send_uplink_direct_transfer_msg */

/*===========================================================================

FUNCTION tdsrrcudt_re_send_uplink_direct_transfer_msg

DESCRIPTION
  This function sends the uplink direct transfer on the uplink DCCH, for
  messages for which we got L2-ACK failure due to re-establishment

DEPENDENCIES
  None.

RETURN VALUE
  void.

===========================================================================*/
static uecomdef_status_e_type tdsrrcudt_re_send_uplink_direct_transfer_msg
(
 uint8                       retransmit_mui,
 rrc_nas_msg_priority_e_type retransmit_msg_priority
)
{
  uint8 count_index;                    /* Count for saved messages */
  tdsrrc_UL_DCCH_Message *msg_ptr;         /* Pointer to uplink DCCH message */ 
  
  rlc_lc_id_type ul_lc_id;              /* Uplink logical channel ID */

  tdsrrcsend_status_e_type send_status;    /* send status of UL message */

  uint8 mui;                            /* Local varibale for MUI */

  rrc_data_req_type *save_temp_ptr=NULL;
  boolean           *save_data_req_status_ptr=NULL;
  boolean           msg_found = FALSE;  

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Attempt to resend UDT for MUI %d,NAS-MSG-Priority %d",retransmit_mui,retransmit_msg_priority);

  //loop thru cs/ps domain saved messages and try and find the message with mui/nas-priority
  //that matches the mui for which we are doing a l2-re-transmission.Also check the mui status
  for ( count_index = 0; count_index < MAX_NAS_MESSAGES_FOR_CS_DOMAIN; count_index++)
  {
    if( (tdsrrcudt_cs_saved_data[count_index].mui == retransmit_mui) &&
        (tdsrrcudt_cs_saved_data[count_index].data_req.priority == retransmit_msg_priority) &&
        (tdsrrcudt_cs_saved_data[count_index].mui_status == TRUE)
        )
    {
      //force data_req_status to true bcos we want to resend this again
      tdsrrcudt_cs_saved_data[count_index].data_req_status = TRUE;
      save_data_req_status_ptr = 
        &tdsrrcudt_cs_saved_data[count_index].data_req_status;
      save_temp_ptr = &tdsrrcudt_cs_saved_data[count_index].data_req;
      msg_found = TRUE;
      break;
    }
  }
  
  if(msg_found != TRUE)
  {
    for ( count_index = 0; count_index < TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN; count_index++)
    {
      if( (tdsrrcudt_ps_saved_data[count_index].mui == retransmit_mui) &&
          (tdsrrcudt_ps_saved_data[count_index].data_req.priority == retransmit_msg_priority) &&
          (tdsrrcudt_ps_saved_data[count_index].mui_status == TRUE)
          )
      {
        //force data_req_status to true bcos we want to resend this again
        tdsrrcudt_ps_saved_data[count_index].data_req_status = TRUE;
        save_data_req_status_ptr = 
          &tdsrrcudt_ps_saved_data[count_index].data_req_status;
        save_temp_ptr = &tdsrrcudt_ps_saved_data[count_index].data_req;
        msg_found = TRUE;
        break;
      }
    }
  }

  //if no mui for which we failed is found,  return error and don't process any further
  if(msg_found == FALSE)
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"Did not find mui %d for nas-priority %d",retransmit_mui,retransmit_msg_priority);
    return FAILURE;
  }
  else
  {
    if(NULL == save_data_req_status_ptr || NULL == save_temp_ptr)
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"mui %d for nas-priority %d msg-ptr null",retransmit_mui,retransmit_msg_priority);
      return FAILURE;
    }

    else if( *(save_data_req_status_ptr) == TRUE)
    {
      if((tdsis_scrr_in_progress) && (tdsrrcscrr_substate != TDSRRCSCRR_INITIAL))
      {
        if((save_temp_ptr->cn_domain_id == RRC_CS_DOMAIN_CN_ID) &&
           (tdsrrcscrr_saved_cs_close_session_req.mui_status != TDSRRCSCRR_MUI_STATUS_FALSE))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SCRR for CS in progress. Discard CS UDT");
          return FAILURE;
        }
        else if((save_temp_ptr->cn_domain_id == RRC_PS_DOMAIN_CN_ID) &&
           (tdsrrcscrr_saved_ps_close_session_req.mui_status != TDSRRCSCRR_MUI_STATUS_FALSE))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SCRR for PS in progress. Discard PS UDT");
          return FAILURE;
        }
      }

     /* Determine the logical channel id for the Uplink DCCH */
      if(save_temp_ptr->priority == RRC_NAS_MSG_HIGH_PRIO)
      {
         ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH,
                                           DCCH_DT_HIGH_PRI_RADIO_BEARER_ID, 
                                           UE_MODE_ACKNOWLEDGED_DATA);
      }
      else
      {
         ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH,
                                           DCCH_DT_LOW_PRI_RADIO_BEARER_ID, 
                                           UE_MODE_ACKNOWLEDGED_DATA);
      }
      /*lint -save -e650 */
      if( (ul_lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND) &&
          (save_temp_ptr->priority == RRC_NAS_MSG_LOW_PRIO) ) 
      {
      /* The logical channel id was not found. If we were looking for the low
       * priority radio bearer (RB 4), try the high priority radio bearer (RB 3).
       */

        ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH,
                                             DCCH_DT_HIGH_PRI_RADIO_BEARER_ID, 
                                             UE_MODE_ACKNOWLEDGED_DATA);

        save_temp_ptr->priority = RRC_NAS_MSG_HIGH_PRIO;
      }
      /*lint -restore */
      if (ul_lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RBs 3 and 4 not found by LCM");

        /*delete CS and PS domain saved info as LC ID is not present    */
        tdsrrcudt_delete_all_saved_data_req(RRC_CS_DOMAIN_CN_ID);
        tdsrrcudt_delete_all_saved_data_req(RRC_PS_DOMAIN_CN_ID);

        /* Reset the state machine */
        tdsrrcudt_substate = TDSRRCUDT_INITIAL;

        return FAILURE;
      }
      else
      {
      /* Build an Uplink Direct Transfer Message. Start by allocating an Uplink 
       * DCCH message. This memory is released by tdsrrcsend_send_ul_sdu().
       */
        msg_ptr = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc( 
          sizeof(struct tdsrrc_UL_DCCH_Message));

       /* Call error fatal if the memory allocation failed
        */
        if (msg_ptr == NULL)
        {
          ERR_FATAL("malloc(%d) failure",sizeof(struct tdsrrc_UL_DCCH_Message),0,0);
        }    

       /* Identify this message as an Uplink Direct Transfer Message
        */
        TDSRRCSET_MSG_TYPE(msg_ptr->message,
               T_tdsrrc_UL_DCCH_MessageType_uplinkDirectTransfer);

        TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.uplinkDirectTransfer);

        if(tdsrrc_get_state() ==  TDSRRC_STATE_CELL_FACH)
        {
          /* Function call to Measurement Control & Report procedure. */
          if(TDSRRCMEAS_RACH_SUCCESSFUL == tdsrrcmeas_append_meas_rpt_on_rach(
             &msg_ptr->message.u.uplinkDirectTransfer.measuredResultsOnRACH, TDSRRC_PROCEDURE_UDT, FALSE))
          {
          /* if Measurement Control & Reportprocedure returns RRC_MEAS_RACH_SUCCESFUL,
             then enable the  bit_mask field */
            TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.uplinkDirectTransfer,
              rrc_UplinkDirectTransfer_,
              measuredResultsOnRACH);
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_MED,"RACH meas is not appended");
          }
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
          if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
          {
            if(SUCCESS == tdsrrcifreq_append_meas_rpt_on_rach(&(msg_ptr->message.u.uplinkDirectTransfer.laterNonCriticalExtensions.v690NonCriticalExtensions.
            uplinkDirectTransfer_v690ext.measuredResultsOnRACHinterFreq),TDSRRC_PROCEDURE_UDT))
            {
              if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR((&msg_ptr->message.u.uplinkDirectTransfer),
                  rrc_UplinkDirectTransfer_,
                  laterNonCriticalExtensions)))
              {
                TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.uplinkDirectTransfer,
                  rrc_UplinkDirectTransfer_,
                  laterNonCriticalExtensions);
                TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.uplinkDirectTransfer.laterNonCriticalExtensions);
              }
              if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR((&msg_ptr->message.u.uplinkDirectTransfer.laterNonCriticalExtensions),
                  rrc_UplinkDirectTransfer_laterNonCriticalExtensions_,
                  v690NonCriticalExtensions)))
              {
                TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.uplinkDirectTransfer.laterNonCriticalExtensions,
                 rrc_UplinkDirectTransfer_laterNonCriticalExtensions_,
                 v690NonCriticalExtensions);
                TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.uplinkDirectTransfer.laterNonCriticalExtensions.v690NonCriticalExtensions);
                TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.uplinkDirectTransfer.laterNonCriticalExtensions.v690NonCriticalExtensions.uplinkDirectTransfer_v690ext);
              }
              TDSRRCSET_R3_MSG_IE_PRESENT(msg_ptr->message.u.uplinkDirectTransfer.laterNonCriticalExtensions.v690NonCriticalExtensions.uplinkDirectTransfer_v690ext, 
                  rrc_UplinkDirectTransfer_v690ext_IEs_,
                  measuredResultsOnRACHinterFreq);
            }
          }
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/

        }


        /* Copy the contents of Saved TDSRRC_DATA_REQ 
        */    
        memscpy(msg_ptr->message.u.uplinkDirectTransfer.nas_Message.data,
          save_temp_ptr->nas_msg.length,
          save_temp_ptr->nas_msg.value, 
          save_temp_ptr->nas_msg.length);

        msg_ptr->message.u.uplinkDirectTransfer.nas_Message.numocts = 
          save_temp_ptr->nas_msg.length;

       /* Fill in the CN Domain id given by the command 
        */

        if(save_temp_ptr->cn_domain_id == RRC_CS_DOMAIN_CN_ID)
        {
          msg_ptr->message.u.uplinkDirectTransfer.cn_DomainIdentity = 
          tdsrrc_CN_DomainIdentity_cs_domain;
        }
        else
        {
          msg_ptr->message.u.uplinkDirectTransfer.cn_DomainIdentity =
            tdsrrc_CN_DomainIdentity_ps_domain;
        }

       /* Send the Uplink Direct Transfer Message on the Uplink DCCH
        */
        send_status = tdsrrcsend_send_ul_sdu_with_mui(TDSRRC_PROCEDURE_UDT,
                                          msg_ptr,
                                          ul_lc_id,
                                          TDSRRCSEND_L2ACK_REQUIRED,
                                          &mui
                                          );

        if (send_status == TDSRRCSEND_SUCCESS)
        {
          switch(save_temp_ptr->cn_domain_id)
          {
            case RRC_CS_DOMAIN_CN_ID :
              /* Check array bounds to keep lint happy */ 
              if(count_index < MAX_NAS_MESSAGES_FOR_CS_DOMAIN)
              {
                tdsrrcudt_cs_saved_data[count_index].mui_status = TRUE;
                tdsrrcudt_cs_saved_data[count_index].mui = mui;
#ifdef FEATURE_TDSCDMA_MTPL_THERMAL_BACKOFF 
                if(TRUE == save_temp_ptr->is_emergency_call)
                {
                  tdsrrcrce_set_rrc_est_cause_for_emergency_call();
                }								
#endif
              }
              else
              {
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid array index %d in tdsrrcudt_cs_saved_data",count_index);
              }
              break;

            case RRC_PS_DOMAIN_CN_ID :
              /* Check array bounds to keep lint happy */ 
              if(count_index < TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN)
              {
                tdsrrcudt_ps_saved_data[count_index].mui_status = TRUE;
                tdsrrcudt_ps_saved_data[count_index].mui = mui;
              }
              else
              {
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid array index %d in tdsrrcudt_ps_saved_data",count_index);
              }
              break;

            default :
              ERR_FATAL("Invalid Domain ID %d",save_temp_ptr->cn_domain_id,0,0);
              break;
          }

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sent Uplink Direct Transfer Message");  
          tdsrrcudt_substate = TDSRRCUDT_WAIT_FOR_L2ACK;
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Error %d sending Uplink Direct Xfer Msg",send_status);

          tdsrrcudt_delete_all_saved_data_req(RRC_CS_DOMAIN_CN_ID);
          tdsrrcudt_delete_all_saved_data_req(RRC_PS_DOMAIN_CN_ID);

          tdsrrcudt_substate = TDSRRCUDT_INITIAL;

        }
        /*set the flag to false.  no need to call delete_data_req as nas_msg.value is
         *already deleted in tdsrrcsend_send_ul_sdu
         */
        *(save_data_req_status_ptr) = FALSE;
      }            
    }
  }

  //all error handling done before and failure returned.. return success if we are here
  return SUCCESS;
}/* tdsrrcudt_send_uplink_direct_transfer_msg */
#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
/*===========================================================================

FUNCTION tdsrrcudt_save_g2w_nas_data_ind

DESCRIPTION
  This function saves the NAS Data Req from the other RAT in case of a HO message
  in the static variable saved_data_req for CS domain

DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcdt_status_e_type.

===========================================================================*/
static tdsrrcdt_status_e_type tdsrrcudt_save_g2w_nas_data_ind
(
  tdsrrc_rr_cmd_type *cmd_ptr                          /* Pointer to RR Command */
)
{
  unsigned char       *payload;                     /* Pointer to the MM payload */
  uint8               index;                        /* Local variable for index */

  /*domain id is hard-coded as we can receive this only in
    CS domain for a GSM to TDS handover*/
  if(tdsrrcudt_check_if_msg_status_can_be_saved(RRC_CS_DOMAIN_CN_ID, &index ) != SUCCESS)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Can't process NAS DATA in CS domain");
    return TDSRRCDT_FAILURE_MINOR;  
  }

  tdsrrcudt_cs_saved_data[index].data_req.cn_domain_id = RRC_CS_DOMAIN_CN_ID;
      

  /* Send the  priority */
  if(cmd_ptr->cmd.interrat_nas_data_ind.SAPI == 0)
  {
  
    tdsrrcudt_cs_saved_data[index].data_req.priority = 
      RRC_NAS_MSG_HIGH_PRIO;
  }
  else if (cmd_ptr->cmd.interrat_nas_data_ind.SAPI == 3)
  {
    tdsrrcudt_cs_saved_data[index].data_req.priority = 
      RRC_NAS_MSG_LOW_PRIO;
  }
  else
  {
    ERR_FATAL("Invalid SAPI is received",0,0,0);
  }
    
  /* Allocate a buffer for the payload. This memory is released by 
   * tdsrrcsend_send_ul_sdu().
   */
  payload = tdsrrc_malloc(cmd_ptr->cmd.interrat_nas_data_ind.l3_message_length);
    
  if (payload == NULL)
  {
    ERR_FATAL("malloc(%d) failure",cmd_ptr->cmd.interrat_nas_data_ind.l3_message_length,0,0);
  }
    
  /* Copy the payload from the MM primitive into the newly allocated memory
   */
  memscpy(payload,
         cmd_ptr->cmd.interrat_nas_data_ind.l3_message_length,
         cmd_ptr->cmd.interrat_nas_data_ind.l3_message,
         cmd_ptr->cmd.interrat_nas_data_ind.l3_message_length);
    
  /* Set the pointer to the payload and the length of the payload in the 
   * Uplink Direct Transfer Message
   */
  tdsrrcudt_cs_saved_data[index].data_req.nas_msg.value = payload;
  tdsrrcudt_cs_saved_data[index].data_req.nas_msg.length = 
    (unsigned short) cmd_ptr->cmd.interrat_nas_data_ind.l3_message_length;
    
  /* set the flag to indicate that DATA_REQ cmd contents are saved and 
   * return success status
   */
  tdsrrcudt_cs_saved_data[index].data_req_status = TRUE;
  
  return TDSRRCDT_SUCCESS;
} /*tdsrrcudt_save_g2w_nas_data_ind*/
/*===========================================================================

FUNCTION tdsrrcudt_process_g2w_nas_data_ind

DESCRIPTION
  This function validates, saves and sends the nas data indication

DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcdt_status_e_type.

===========================================================================*/
static tdsrrcdt_status_e_type tdsrrcudt_process_g2w_nas_data_ind
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to RRC Command */
)
{
  tdsrrc_rr_cmd_type * nas_msg_ptr;        /* ptr to nas msg typecast   */

  mm_cmd_type *mm_cmd_ptr;              /* mm cmd ptr holder         */

  /*typecast the message to rr type*/
  /*lint -e740*/
  nas_msg_ptr = (tdsrrc_rr_cmd_type *) cmd_ptr;
  /*lint +e740*/
  
  /*set cs domain to open as we have successfully moved to DCH mode due to
    an GtoW Interrat Handover */
  tdsrrcdt_int_data.cs_domain_open = TRUE;
    
  /*if there are no mm messages to send at this time, then send activation ind
  to nas*/
  if(nas_msg_ptr->cmd.interrat_nas_data_ind.l3_message_length == 0)
  {
  /* Get a buffer from MM task */
    if((mm_cmd_ptr = mm_tdsrrc_get_cmd_buf(RRC_ACTIVATION_IND)) == NULL)
    {
      ERR_FATAL("No MM buffer for RRC_ACTIVATION_IND",0,0,0);
    }
    mm_cmd_ptr->cmd.rrc_activation_ind.status = MM_AS_INTERRAT_HANDOVER;

#ifdef FEATURE_DUAL_SIM
    mm_cmd_ptr->cmd.rrc_activation_ind.as_id = tdsrrc_get_as_id();
#endif

    /* Put the command on MM queue */
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(mm_cmd_ptr);
    }
    mm_put_cmd(mm_cmd_ptr);

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_ACTIVATION_IND sent to MM");

    /*update the cause for activation indication*/
    tdsrrcmcm_update_tds_activation_cause(RRCMCM_INTERRAT_HANDOVER_G2TD);

    return TDSRRCDT_SUCCESS; 
  }

  /*check to see if state is CELL_DCH or not*/
  if(tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"NAS_DATA_IND valid only in DCH");
    return TDSRRCDT_FAILURE_MINOR;
  }
    
  if(TDSRRCDT_SUCCESS != tdsrrcudt_save_g2w_nas_data_ind(nas_msg_ptr))
  {
    /* save udt message */
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Failed to Save NAS Data Ind in : %d,%d",tdsrrc_get_state(),tdsrrcudt_substate);
    return TDSRRCDT_FAILURE_MINOR;
  }    
  
  /* Build and send the Uplink Direct Transfer to Send Chain */
  tdsrrcudt_send_uplink_direct_transfer_msg();
  
  return TDSRRCDT_SUCCESS;          
}/*tdsrrcudt_process_g2w_nas_data_ind*/

#endif /* FEATURE_INTERRAT_HANDOVER_G2TD */
#ifdef FEATURE_LTE_TO_TDSCDMA
/*===========================================================================

FUNCTION tdsrrcudt_process_open_domain_ind_from_iho

DESCRIPTION
  This function processes open domain indication from IHO and sends
  activation indication to NAS.

DEPENDENCIES
  None.

RETURN VALUE
  rrcdt_status_e_type.

===========================================================================*/
static void tdsrrcudt_process_open_domain_ind_from_iho
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to RRC Command */
)
{
  mm_cmd_type *mm_cmd_ptr;              /* mm cmd ptr holder         */

  if(cmd_ptr->cmd.rrc_open_domain_ind.domain == TDSRRC_CS_DOMAIN)
  {
    /*set cs domain to open as we have successfully moved to DCH mode due to
      an LtoW SRVCC */
    tdsrrcdt_int_data.cs_domain_open = TRUE;
  }
  else if(cmd_ptr->cmd.rrc_open_domain_ind.domain == TDSRRC_PS_DOMAIN)
  {
    /*set ps domain to open as we have successfully moved to DCH mode due to
      an LtoW Interrat PS Handover */
    tdsrrcdt_int_data.ps_domain_open = TRUE;
  }
  else if(cmd_ptr->cmd.rrc_open_domain_ind.domain == TDSRRC_CS_PLUS_PS_DOMAIN)
  {
    /*set cs and ps domain to open as we have successfully moved to DCH mode due to
      an LtoW SRVCC */
    tdsrrcdt_int_data.cs_domain_open = TRUE;
    tdsrrcdt_int_data.ps_domain_open = TRUE;
  }
    
  /* Unsync the priority before sending Activation Ind for X2T HO */
 //TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: Un-sync priority");
 //tdrm_unsync_reason();

  /* Get a buffer from MM task */
  if((mm_cmd_ptr = mm_tdsrrc_get_cmd_buf(RRC_ACTIVATION_IND)) == NULL)
  {
    ERR_FATAL("No MM buffer for RRC_ACTIVATION_IND",0,0,0);
  }

  if(cmd_ptr->cmd.rrc_open_domain_ind.domain == TDSRRC_CS_PLUS_PS_DOMAIN)
  {
    mm_cmd_ptr->cmd.rrc_activation_ind.status = MM_AS_LTOW_SRVCC_CS_PS_HANDOVER;
  }
  else if(cmd_ptr->cmd.rrc_open_domain_ind.domain == TDSRRC_CS_DOMAIN)
  {
    mm_cmd_ptr->cmd.rrc_activation_ind.status = MM_AS_LTOW_SRVCC_CS_HANDOVER;
  }
  else
  {
    mm_cmd_ptr->cmd.rrc_activation_ind.status = MM_AS_INTERRAT_PS_HANDOVER;
  }
  
  #ifdef FEATURE_DUAL_SIM
  mm_cmd_ptr->cmd.rrc_activation_ind.as_id = tdsrrc_get_as_id();
  #endif
  
  /* Put the command on MM queue */
  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_mm_put_cmd(mm_cmd_ptr);
  }
  mm_put_cmd(mm_cmd_ptr);
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_ACTIVATION_IND sent to MM");
  
  /*update the cause for activation indication*/
  tdsrrcmcm_update_tds_activation_cause(RRCMCM_INTERRAT_HANDOVER_G2TD);
  }
#endif


/*===========================================================================

FUNCTION tdsrrcudt_send_nas_data_ind

DESCRIPTION
  This function sends TDSRR_NAS_DATA_IND if there is any NAS message pending
  to transmitted or pending to be acknowledged from UTRAN.


DEPENDENCIES
  None.

RETURN VALUE
  void.

===========================================================================*/
static void tdsrrcudt_send_nas_data_ind
(
 void
)
{
  uint8 count;                          /* Local variable to store index */
  tdsrr_nas_data_ind_type nas_data_ind;    /* Local variable to store
                                           NAS message to be sent to RR */

  memset(&nas_data_ind,0,sizeof(tdsrr_nas_data_ind_type)); 
                                           
  switch(tdsrrcudt_substate)
  {
    case TDSRRCUDT_INITIAL:
      /* No Pending NAS message */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No Pending UDT messages for TDSRR_NAS_DATA_IND");

      /* Set the message length to 0 */
      nas_data_ind.nas_data.message_length = 0;
  
      /* Send the message to RR */
      tdsrr_send_rrc_message_to_rr_internal((void *)(&nas_data_ind),
                                 TDSRR_NAS_DATA_IND,
                                 sizeof(tdsrr_nas_data_ind_type));
      
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sent TDSRR_NAS_DATA_IND terminator(0 message length)");
      break;

    case TDSRRCUDT_WAIT_FOR_CELL_UPDATE_COMP_CNF:                               
    case TDSRRCUDT_WAIT_FOR_L2ACK:
      if(SUCCESS == tdsrrcudt_check_saved_message_status() )
      {
        /* Copy the saved NAS message for CS */
        for (count = 0; count < MAX_NAS_MESSAGES_FOR_CS_DOMAIN; count++ )
        {
          /* Send the saved message to RR only if it is active and 
             is not of zero length */
          if(((tdsrrcudt_cs_saved_data[count].mui_status == TRUE) ||
              (tdsrrcudt_cs_saved_data[count].data_req_status == TRUE)) &&
              (tdsrrcudt_cs_saved_data[count].data_req.nas_msg.length != 0))
          {
            if (tdsrrcudt_cs_saved_data[count].data_req.nas_msg.length > 255)
            {
              ERR_FATAL("Len of NAS msg > 255",0,0,0);
            }

            nas_data_ind.nas_data.message_length = (uint8)
                tdsrrcudt_cs_saved_data[count].data_req.nas_msg.length;

            memscpy(nas_data_ind.nas_data.payload,
                   nas_data_ind.nas_data.message_length,
                   tdsrrcudt_cs_saved_data[count].data_req.nas_msg.value,
                   tdsrrcudt_cs_saved_data[count].data_req.nas_msg.length );

            /* Send the message to RR */
            tdsrr_send_rrc_message_to_rr_internal((void *)(&nas_data_ind),
                                       TDSRR_NAS_DATA_IND,
                                       sizeof(tdsrr_nas_data_ind_type));

            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sent TDSRR_NAS_DATA_IND message length: %d",nas_data_ind.nas_data.message_length);
          } 
        }

        /* Copy the saved NAS message for PS */
        for (count = 0; count < TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN; count++ )
        {
          /* Send the saved message to RR only if it is active and 
             is not of zero length */
          if(((tdsrrcudt_ps_saved_data[count].mui_status == TRUE) ||
              (tdsrrcudt_ps_saved_data[count].data_req_status == TRUE)) &&
              (tdsrrcudt_ps_saved_data[count].data_req.nas_msg.length != 0))
          {
            if(tdsrrcudt_ps_saved_data[count].data_req.nas_msg.length > 255)
            {
              ERR_FATAL("Len of NA msg > 255",0,0,0);
            }

            nas_data_ind.nas_data.message_length = (uint8)
                tdsrrcudt_ps_saved_data[count].data_req.nas_msg.length;

            memscpy(nas_data_ind.nas_data.payload,
                   nas_data_ind.nas_data.message_length,
                   tdsrrcudt_ps_saved_data[count].data_req.nas_msg.value,
                   tdsrrcudt_ps_saved_data[count].data_req.nas_msg.length );

            /* Send the message to RR */
            tdsrr_send_rrc_message_to_rr_internal((void *)(&nas_data_ind),
                                       TDSRR_NAS_DATA_IND,
                                       sizeof(tdsrr_nas_data_ind_type));

            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sent TDSRR_NAS_DATA_IND message length: %d",nas_data_ind.nas_data.message_length);
          }
        }

        /* Since we have sent all the saved messages to RR, clear the arrays */
        tdsrrcudt_delete_all_saved_data_req(RRC_CS_DOMAIN_CN_ID);
        tdsrrcudt_delete_all_saved_data_req(RRC_PS_DOMAIN_CN_ID);
        tdsrrcudt_substate = TDSRRCUDT_INITIAL;
      }
      
      /* The last message always has to be NULL message which will act as the 
         terminator for NAS_DATA_IND. Set the message length to 0 */
      nas_data_ind.nas_data.message_length = 0;

      /* Send the message to RR */
      tdsrr_send_rrc_message_to_rr_internal((void *)(&nas_data_ind),
                                TDSRR_NAS_DATA_IND,
                                sizeof(tdsrr_nas_data_ind_type));

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sent TDSRR_NAS_DATA_IND terminator(0 message length)");
      break;

    default:  
      ERR_FATAL("Invalid RRCUDT substate: %d ",tdsrrcudt_substate,0,0);
      break;
   }   
}/* tdsrrcudt_send_nas_data_ind */
/*===========================================================================

FUNCTION tdsrrcudt_process_udt_initial_substate

DESCRIPTION
  This function processes all events that are  dispatched
  to Uplink Direct Transfer procedure in TDSRRCUDT_INITIAL substate

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

static void tdsrrcudt_process_udt_initial_substate
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to RRC Command */
)
{
  sys_modem_as_id_e_type as_id = tdsrrc_get_as_id();
  
  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    /* happens when a state change ind is received in idle state.
     * do nothing when it happens
     */
    case TDSRRC_DATA_REQ:
      if(TDSRRCDT_SUCCESS != tdsrrcudt_validate_data_req(cmd_ptr))
      {
        /* If TDSRRC_DATA_REQ is invalid primitive, then trash the same */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Validation of TDSRRC_DATA_REQ cmd Failed");
        return;
      }
    
      if(TDSRRCDT_SUCCESS != tdsrrcudt_save_data_req(cmd_ptr))
      {
        /* save udt message */
        /*lint -e666 */
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Failed to Save Data Req in : %d,%d",tdsrrc_get_state(),tdsrrcudt_substate);
        /*lint +e666 */
        return;
      }
      if (cmd_ptr->cmd.data_req.cn_domain_id == RRC_PS_DOMAIN_CN_ID)
      {
          /* Both PS and CS hasn't receive the RAB setup. */
          if ((mn_voice_call_vote_for_reselection(as_id) == NAS_VOTE_FOR_RESEL_FALSE)&&
              (sm_ps_call_vote_for_reselection(as_id) == NAS_VOTE_FOR_RESEL_FALSE)&&
              (TRUE == tdsrrc_hold_ps_tmr_expired))
          {
            if (tdsrrc_nv_hold_ps_tmr_value > 0)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Hold PS: hold %d ms", tdsrrc_nv_hold_ps_tmr_value*10);
              tdsrrctmr_start_timer(TDSRRCTMR_HOLD_PS_TIMER, tdsrrc_nv_hold_ps_tmr_value*10);
              tdsrrc_hold_ps_tmr_expired = FALSE;
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Hold PS: don't hold PS");
            }
          }
      }
      /* Check RRC state and call functions appropriately.
       */
      switch(tdsrrc_get_state())
      {
        case TDSRRC_STATE_URA_PCH:
        case TDSRRC_STATE_CELL_PCH:
        case TDSRRC_STATE_CELL_FACH:
        case TDSRRC_STATE_CELL_DCH:
          //This queries whether a CELL Update procedure is active.If Cell Update
          //is active, it registers for the complete and enters the substate
          //RRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF. 
          if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status(TDSRRC_PROCEDURE_UDT) )
          {
            if(tdsis_ue_oos_and_em_call_attempted(cmd_ptr, TDSRRC_PROCEDURE_UDT))
            {
              tdsrrcudt_substate = TDSRRCUDT_INITIAL;  
            }
            else
            {
              /* Wait until Cell update procedure is completed */
              tdsrrcudt_substate = TDSRRCUDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;  
            }
          }
          else /* Cell Update procedure is inactive */
          {
            /* Build and send the Uplink Direct Transfer to Send Chain */
            tdsrrcudt_send_uplink_direct_transfer_msg();
          }
          break;
    
        default:
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"UDT:Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcudt_substate);
          break;
      }
      break;

    case TDSRRC_NAS_DATA_TRANSFER_IND:
      tdsrrcudt_send_nas_data_ind();
      break;

#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)  
    /*lint -e408*/  
    case TDSRRC_NAS_DATA_IND:
    /*lint +e408*/
      (void)tdsrrcudt_process_g2w_nas_data_ind(cmd_ptr);
      break;
      
#endif /*FEATURE_INTERRAT_HANDOVER_G2TD*/
#ifdef FEATURE_LTE_TO_TDSCDMA
    case TDSRRC_IHO_OPEN_DOMAIN_IND:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received TDSRRC_IHO_OPEN_DOMAIN_IND");
      tdsrrcudt_process_open_domain_ind_from_iho(cmd_ptr);
      break;
#endif /*FEATURE_LTE_TO_TDSCDMA */


    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d for UDT in substate:%d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcudt_substate);
      break;
  }/*end of switch stmt*/
} /* tdsrrcudt_process_udt_initial_substate */

/*===========================================================================

FUNCTION tdsrrcudt_process_udt_wait_for_cell_update_comp_cnf_substate

DESCRIPTION
  This function processes all events that are  dispatched
  to Uplink Direct Transfer procedure in TDSRRCUDT_WAIT_FOR_CELL_UPDATE_COMP_CNF
  substate

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

static void tdsrrcudt_process_udt_wait_for_cell_update_comp_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{
  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
    case TDSRRC_HOLD_PS_TIMER_EXPIRED_IND:
      /* Build and send the Uplink Direct Transfer to Send Chain */
      tdsrrcudt_send_uplink_direct_transfer_msg();

      break;

    case TDSRRC_DATA_REQ:
      if(tdsis_ue_oos_and_em_call_attempted(cmd_ptr, TDSRRC_PROCEDURE_UDT))
      {
        tdsrrcudt_substate = TDSRRCUDT_INITIAL;  
      }
      else
      {
        /*we may have got a save req for a different CN domain.  try
         *to save the data_req command*/
        (void) tdsrrcudt_save_data_req( cmd_ptr );
      }
      break;


    case TDSRRC_NAS_DATA_TRANSFER_IND:
      /*Copy the saved NAS message */
      tdsrrcudt_send_nas_data_ind();
      /* Reset the state machine */
      tdsrrcudt_substate = TDSRRCUDT_INITIAL;  
      break;
    
    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"UDT:Invalid Event: %d in substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcudt_substate);
      break;
  }
 
} /* tdsrrcudt_process_udt_wait_for_cell_update_comp_cnf_substate */
/*===========================================================================

FUNCTION tdsrrcudt_process_udt_wait_for_l2ack_substate

DESCRIPTION
  This function processes all events that are  dispatched
  to Initial Direct Transfer procedure in TDSRRCUDT_WAIT_FOR_L2ACK
  substate.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

static void tdsrrcudt_process_udt_wait_for_l2ack_substate
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{
  tdsrrc_RB_Identity rb_id;                /* Local variable to store RB Id */

  rrc_nas_msg_priority_e_type nas_msg_priority;
  sys_modem_as_id_e_type as_id = tdsrrc_get_as_id();
  
  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_DOWNLINK_L2ACK_IND:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2 ACK for UL DirecT xfer Mesg Rcvd.Status: %d",cmd_ptr->cmd.downlink_l2ack_ind.status);

      /* Check for the MUI received */
      rb_id = tdsrrclcm_get_ul_rb_id(cmd_ptr->cmd.downlink_l2ack_ind.lc_id);

      if((rb_id < DCCH_DT_HIGH_PRI_RADIO_BEARER_ID) || 
         (rb_id > DCCH_DT_LOW_PRI_RADIO_BEARER_ID))
      {
        ERR_FATAL("L2 ACK routing is incorrect",0,0,0);
      }

      if(rb_id == DCCH_DT_HIGH_PRI_RADIO_BEARER_ID)
      {
        nas_msg_priority = RRC_NAS_MSG_HIGH_PRIO; 
      }
      else
      {
        nas_msg_priority = RRC_NAS_MSG_LOW_PRIO;
      }

      if(cmd_ptr->cmd.downlink_l2ack_ind.status == SUCCESS)
      {
        if(SUCCESS == tdsrrcudt_delete_saved_data_req((rrc_nas_msg_priority_e_type) nas_msg_priority,
                                                    cmd_ptr->cmd.downlink_l2ack_ind.mui))
        {
          if(FAILURE == tdsrrcudt_check_saved_mui_status())
          {
            tdsrrcudt_substate = TDSRRCUDT_INITIAL;
          }
          else
          {
            tdsrrcudt_substate = TDSRRCUDT_WAIT_FOR_L2ACK;
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unexpected L2 ACK rcvd");
          /* Do nothing */
        }    
      }
      else /* L2 Failed to transmit the message */
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2 Ack failed for UDT.Failure reason %d",cmd_ptr->cmd.downlink_l2ack_ind.failure_reason);
         
        /* Before re-transmitting the message,check to see if we are waiting for
        L2 Ack for any other UDT message(s) of the same domain.If so,it might 
        be still  sitting in the WM and may go out before this re-trasmitted 
        message and hence the order of delivery would be reversed.This is not 
        desirable,hence purge the WM so that serialization is maintained */

        /* Do not purge WM if the L2 Ack failure is because of WM purging.This 
           would result in an infinite loop */  
        if(cmd_ptr->cmd.downlink_l2ack_ind.failure_reason != WM_PURGE_REQ)
        {
          uint32 count = 0;
          uint32 pending_count = 0;
          rrc_cn_domain_identity_e_type domain_id;

          /* Get the domain id of the message */
          domain_id = tdsrrcudt_get_cn_domain_for_mui(cmd_ptr->cmd.downlink_l2ack_ind.mui);
  
          if(domain_id == RRC_CS_DOMAIN_CN_ID)
          {
            for (count = 0;count < MAX_NAS_MESSAGES_FOR_CS_DOMAIN;count++ )
            {
              if(tdsrrcudt_cs_saved_data[count].mui_status == TRUE)
              {
                pending_count++; 
              }
            }
          }
          else /* PS domain */
          {
            for (count = 0;count < TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN;count++ )
            {
              if(tdsrrcudt_ps_saved_data[count].mui_status == TRUE)
              {
                pending_count++; 
              }
            }
          }
  
          /* Check to see if pending count is greater than 1.i.e. another message
             is waiting for a L2 ack in the same domain apart from the one that just
             got the L2 Ack(failure) */
          if(pending_count > 1)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Need to purge WM3 due to failed L2 Ack");
            tdsrrcdt_purge_rlc_wm_rb3_rb4(TRUE,FALSE,TDSRRC_PROCEDURE_UDT);
          }
        }
        /* Re-transmission of the message */
        if(FAILURE == tdsrrcudt_re_send_uplink_direct_transfer_msg(cmd_ptr->cmd.downlink_l2ack_ind.mui, 
                                                                nas_msg_priority))
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Retransmission attempt failed");
          (void)tdsrrcudt_delete_saved_data_req((rrc_nas_msg_priority_e_type) nas_msg_priority,
                                              cmd_ptr->cmd.downlink_l2ack_ind.mui);
        }
      }
      break;

    case TDSRRC_DATA_REQ:
      if(TDSRRCDT_SUCCESS != tdsrrcudt_validate_data_req(cmd_ptr))
      {
        /* If TDSRRC_DATA_REQ is invalid primitive, then trash the same */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Validation of TDSRRC_DATA_REQ cmd Failed");
        return;
      }
      
      if(TDSRRCDT_SUCCESS != tdsrrcudt_save_data_req(cmd_ptr))
      {
        /* save udt message */
        /*lint -e666*/
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Failed to Save Data Req in : %d,%d",tdsrrc_get_state(),tdsrrcudt_substate);
        /*lint +e666*/
        return;
      }
      
      if (cmd_ptr->cmd.data_req.cn_domain_id == RRC_PS_DOMAIN_CN_ID)
      {
          /* Both PS and CS hasn't receive the RAB setup. */
          if ((mn_voice_call_vote_for_reselection(as_id) == NAS_VOTE_FOR_RESEL_FALSE)&&
              (sm_ps_call_vote_for_reselection(as_id) == NAS_VOTE_FOR_RESEL_FALSE)&&
              (TRUE == tdsrrc_hold_ps_tmr_expired))
          {
            if (tdsrrc_nv_hold_ps_tmr_value > 0)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Hold PS: hold %d ms", tdsrrc_nv_hold_ps_tmr_value*10);
              tdsrrctmr_start_timer(TDSRRCTMR_HOLD_PS_TIMER, tdsrrc_nv_hold_ps_tmr_value*10);
              tdsrrc_hold_ps_tmr_expired = FALSE;
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Hold PS: don't hold PS");
            }
          }
      }
      
      /* Check RRC state and call functions appropriately.
       */
      switch(tdsrrc_get_state())
      {
        case TDSRRC_STATE_CELL_FACH:
          //This queries whether a CELL Update procedure is active.If Cell Update
          //is active, it registers for the complete and enters the substate
          //RRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF. 
    
          if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status(TDSRRC_PROCEDURE_UDT) )
          {
            if(tdsis_ue_oos_and_em_call_attempted(cmd_ptr, TDSRRC_PROCEDURE_UDT))
            {
              tdsrrcudt_substate = TDSRRCUDT_INITIAL;  
            }
            else
            {
              /* Wait until Cell update procedure is completed */
              tdsrrcudt_substate = TDSRRCUDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;  
            }
          }
          else /* Cell Update procedure is inactive */
          {
            /* Build and send the Uplink Direct Transfer to Send Chain */
            tdsrrcudt_send_uplink_direct_transfer_msg();
          }
          break;

        case TDSRRC_STATE_CELL_DCH:
          /* Build and send the Uplink Direct Transfer to Send Chain */
          tdsrrcudt_send_uplink_direct_transfer_msg();
          break;

        /* RRCDT should not recieve TDSRRC_DATA_REQ in other RRC states */
        default:
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"UDT:Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcudt_substate);
          break;
      }
      break;

    case TDSRRC_NAS_DATA_TRANSFER_IND:
      /*Copy the saved NAS message */
      tdsrrcudt_send_nas_data_ind();
      /* Reset the state machine */
      tdsrrcudt_substate = TDSRRCUDT_INITIAL;  
      break;

#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
    /*lint -e408*/    
    case TDSRRC_NAS_DATA_IND:
    /*lint +e408*/
      /*process the nas data indication*/
      (void)tdsrrcudt_process_g2w_nas_data_ind(cmd_ptr);
      break;
      
#endif /*FEATURE_INTERRAT_HANDOVER_G2TD*/

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d for IDT in substate:%d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcudt_substate);
      break;
  }
 
} /* tdsrrcudt_process_udt_wait_for_l2ack_substate */

/*===========================================================================

FUNCTION tdsrrcudt_process_disconnected_state

DESCRIPTION
  This function processes all events that are dispatched to Uplink direct
  Transfer procedure in Disconnected state of Idle mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcudt_process_disconnected_state
(
void
)
{
  switch (tdsrrcudt_substate)
  {
    case TDSRRCUDT_INITIAL:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing UDT in disconnected state. Force iitialise RCE and SIB modules");
      tdsrrcrce_force_initialize(TRUE);
      tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_SIB7_UPDATE);
      break;

    default:           
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"UDT:Invalid substate: %d in DISCONNECTED",tdsrrcudt_substate);
      break;
  }  
}/* tdsrrcudt_process_disconnected_state */

/*===========================================================================

FUNCTION tdsrrcudt_process_cell_fach_state

DESCRIPTION
  This function processes all events that are dispatched to Uplink direct
  Transfer procedure in CELL_FACH state of Connected mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcudt_process_cell_fach_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  switch (tdsrrcudt_substate)
  {
    case TDSRRCUDT_INITIAL:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_DATA_REQ:
        case TDSRRC_NAS_DATA_TRANSFER_IND:
          tdsrrcudt_process_udt_initial_substate(cmd_ptr);
          break;
        case TDSRRC_HOLD_PS_TIMER_EXPIRED_IND:
          tdsrrc_hold_ps_tmr_expired = TRUE;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Hold PS:Timer out at Substate: %d",tdsrrcudt_substate);
          if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_UDT ))
          {
            
            tdsrrcudt_substate = TDSRRCUDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;
          }
          else
          {
            tdsrrcudt_send_uplink_direct_transfer_msg();
          }

          break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"UDT:Invalid event in Substate: %d",tdsrrcudt_substate);
          break;
      }
      break;

    case TDSRRCUDT_WAIT_FOR_CELL_UPDATE_COMP_CNF: 
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        /* we can either get a complete confirmation or get a data request
         * for a different CN domain.  Process both in wait for cell update comp cnf
         */
        case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
        case TDSRRC_DATA_REQ:
        case TDSRRC_NAS_DATA_TRANSFER_IND:
          tdsrrcudt_process_udt_wait_for_cell_update_comp_cnf_substate(cmd_ptr);
          break;
        case TDSRRC_DOWNLINK_L2ACK_IND:
          tdsrrcudt_process_l2_ack_cell_udpate_state(cmd_ptr );
          break;
        case TDSRRC_HOLD_PS_TIMER_EXPIRED_IND:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Hold PS:Timer out at Substate: %d",tdsrrcudt_substate);
          tdsrrc_hold_ps_tmr_expired = TRUE;
          if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_UDT ))
          {
            tdsrrcudt_substate = TDSRRCUDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;
          }
          break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"UDT:Invalid event in Substate: %d",tdsrrcudt_substate);
          break;
      }
      break;

    case TDSRRCUDT_WAIT_FOR_L2ACK:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_DOWNLINK_L2ACK_IND:
        case TDSRRC_DATA_REQ:
        case TDSRRC_NAS_DATA_TRANSFER_IND:
          tdsrrcudt_process_udt_wait_for_l2ack_substate(cmd_ptr);
          break;
        case TDSRRC_HOLD_PS_TIMER_EXPIRED_IND:
          tdsrrc_hold_ps_tmr_expired = TRUE;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Hold PS:Timer out at Substate: %d",tdsrrcudt_substate);
          if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_UDT ))
          {
            tdsrrcudt_substate = TDSRRCUDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;
          }
          else
          {
            tdsrrcudt_send_uplink_direct_transfer_msg();
          }
        break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"UDT:Invalid event in Substate:%d ",tdsrrcudt_substate);
          break;
      }
      break;

    default:           
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"UDT:Invalid substate:%d CELL_FACH",tdsrrcudt_substate);
      break;
  }  
}/* tdsrrcudt_process_cell_fach_state */
/*===========================================================================

FUNCTION tdsrrcudt_process_cell_dch_state

DESCRIPTION
  This function processes all events that are dispatched to Initial direct
  Transfer procedure in CELL_DCH state of Connected mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcudt_process_cell_dch_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  switch (tdsrrcudt_substate)
  {
    case TDSRRCUDT_INITIAL:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_DATA_REQ:
        case TDSRRC_NAS_DATA_TRANSFER_IND:

#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
        /*lint -e408*/ 
        case TDSRRC_NAS_DATA_IND:
        /*lint +e408*/ 
#endif
#ifdef FEATURE_LTE_TO_TDSCDMA
        case TDSRRC_IHO_OPEN_DOMAIN_IND:
#endif

          tdsrrcudt_process_udt_initial_substate(cmd_ptr);
          break;
        case TDSRRC_HOLD_PS_TIMER_EXPIRED_IND:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Hold PS:Timer out at Substate: %d",tdsrrcudt_substate);
          tdsrrc_hold_ps_tmr_expired = TRUE;

          if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_UDT ))
          {
            tdsrrcudt_substate = TDSRRCUDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;
          }
          else
          {
            tdsrrcudt_send_uplink_direct_transfer_msg();
          }
          
        break;
        
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"UDT:Invalid event in Substate: %d",tdsrrcudt_substate);
          break;
      }
      break;

    case TDSRRCUDT_WAIT_FOR_CELL_UPDATE_COMP_CNF: 
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        /* we can either get a complete confirmation or get a data request
         * for a different CN domain.  Process both in wait for cell update comp cnf
         */
        case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
        case TDSRRC_DATA_REQ:
        case TDSRRC_NAS_DATA_TRANSFER_IND:
          tdsrrcudt_process_udt_wait_for_cell_update_comp_cnf_substate(cmd_ptr);
          break;
        case TDSRRC_DOWNLINK_L2ACK_IND:
          tdsrrcudt_process_l2_ack_cell_udpate_state(cmd_ptr );
          break;
        case TDSRRC_HOLD_PS_TIMER_EXPIRED_IND:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Hold PS:Timer out at Substate: %d",tdsrrcudt_substate);
          tdsrrc_hold_ps_tmr_expired = TRUE;
          if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_UDT ))
          {
            tdsrrcudt_substate = TDSRRCUDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;
          }
          break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"UDT:Invalid event in Substate: %d",tdsrrcudt_substate);
          break;
      }
      break;

    case TDSRRCUDT_WAIT_FOR_L2ACK:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_DOWNLINK_L2ACK_IND:
        case TDSRRC_DATA_REQ:
        /*lint -e408*/ 
        case TDSRRC_NAS_DATA_TRANSFER_IND:
        /*lint +e408*/ 

#ifdef FEATURE_INTERRAT_HANDOVER_G2TD
        /*lint -e408*/ 
        case TDSRRC_NAS_DATA_IND:
        /*lint +e408*/ 
#endif
          tdsrrcudt_process_udt_wait_for_l2ack_substate(cmd_ptr);
          break;
        case TDSRRC_HOLD_PS_TIMER_EXPIRED_IND:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Hold PS:Timer out at Substate: %d",tdsrrcudt_substate);
          tdsrrc_hold_ps_tmr_expired = TRUE;
          
          if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_UDT ))
          {
            tdsrrcudt_substate = TDSRRCUDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;
          }
          else
          {
            tdsrrcudt_send_uplink_direct_transfer_msg();
          }

        break;
        
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"UDT:Invalid event in Substate:%d ",tdsrrcudt_substate);
          break;
      }
      break;

    default:           
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"UDT:Invalid substate: %d in CELL_DCH",tdsrrcudt_substate);
      break;
  }  
}/* tdsrrcudt_process_cell_dch_state */

/*===========================================================================

FUNCTION tdsrrcudt_process_cell_ura_pch_state

DESCRIPTION
  This function processes all events that are dispatched to Initial direct
  Transfer procedure in either in CELL_PCH or URA_PCH state of Connected mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcudt_process_cell_ura_pch_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  switch (tdsrrcudt_substate)
  {
    case TDSRRCUDT_INITIAL:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_DATA_REQ:
          tdsrrcudt_process_udt_initial_substate(cmd_ptr);
          break;
        case TDSRRC_HOLD_PS_TIMER_EXPIRED_IND:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Hold PS:Timer out at Substate: %d",tdsrrcudt_substate);
          tdsrrc_hold_ps_tmr_expired = TRUE;
          if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status(TDSRRC_PROCEDURE_UDT) )
          {
            /* Wait until Cell update procedure is completed */
            tdsrrcudt_substate = TDSRRCUDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;  
          }
          else /* Cell Update procedure is inactive */
          {
              /* Build and send the Uplink Direct Transfer to Send Chain */
              tdsrrcudt_send_uplink_direct_transfer_msg();
          }
          break;
        
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"UDT:Invalid event in Substate: %d",tdsrrcudt_substate);

          break;
      }
      break;

    case TDSRRCUDT_WAIT_FOR_CELL_UPDATE_COMP_CNF:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Hold PS:Timer out at Substate: %d",tdsrrcudt_substate);
      if (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_HOLD_PS_TIMER_EXPIRED_IND)
      {
        tdsrrc_hold_ps_tmr_expired = TRUE;
      }
      if (TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status( TDSRRC_PROCEDURE_UDT ))
      {
         
         tdsrrcudt_substate = TDSRRCUDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;
      }
      else

      {
        tdsrrcudt_process_udt_wait_for_cell_update_comp_cnf_substate(cmd_ptr);
      }
      break;

    case TDSRRCUDT_WAIT_FOR_L2ACK:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_DATA_REQ:
          tdsrrcudt_process_udt_wait_for_l2ack_substate(cmd_ptr);
          break;
        case TDSRRC_HOLD_PS_TIMER_EXPIRED_IND:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Hold PS:Timer out at Substate: %d",tdsrrcudt_substate);
          tdsrrc_hold_ps_tmr_expired = TRUE;
          if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status(TDSRRC_PROCEDURE_UDT) )
          {
            /* Wait until Cell update procedure is completed */
            tdsrrcudt_substate = TDSRRCUDT_WAIT_FOR_CELL_UPDATE_COMP_CNF;  
          }
          else /* Cell Update procedure is inactive */
          {
              /* Build and send the Uplink Direct Transfer to Send Chain */
              tdsrrcudt_send_uplink_direct_transfer_msg();
          }
          break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"UDT:Invalid event in Substate: %d",tdsrrcudt_substate);
          break;
      }
      break;

    default:           
      if (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_HOLD_PS_TIMER_EXPIRED_IND)
      {
        tdsrrc_hold_ps_tmr_expired = TRUE;
      }
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"UDT:Invalid substate: %d in PCH: %d",tdsrrcudt_substate);
      break;
  }  
}/* tdsrrcudt_process_cell_ura_pch_state */
/*===========================================================================

FUNCTION tdsrrcudt_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the ininitial direct transfer
  procedure.
  The following events are handled by this procedure:

    TDSRRC_DATA_REQ Primitive (received from MM)
    TDSRRC_CELL_UPDATE_COMPLETE_CNF primitive (received from CELL UPDATE Procedure)
    TDSRRC_NAS_DATA_TRANSFER_IND primitive from RR
    TDSRRC_DOWNLINK_L2ACK_IND primitive from RLC
#endif

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcudt_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
)
{
 
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  if(TDSRRC_STATE_CHANGE_IND == cmd_ptr->cmd_hdr.cmd_id)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"State-Change-Ind udt-state %d in rrc-state %d",tdsrrcudt_substate,rrc_state);
    //Any pending message memory needs to be de-allocated here 
    tdsrrctmr_stop_timer(TDSRRCTMR_HOLD_PS_TIMER);
    tdsrrc_hold_ps_tmr_expired = TRUE;
    tdsrrcudt_delete_all_saved_data_req(RRC_CS_DOMAIN_CN_ID);
    tdsrrcudt_delete_all_saved_data_req(RRC_PS_DOMAIN_CN_ID);
    tdsrrcudt_substate = TDSRRCUDT_INITIAL;  
  }
  else
  {
    switch (rrc_state)
    {
      case TDSRRC_STATE_DISCONNECTED: 
        tdsrrcudt_process_disconnected_state();
        break;

      case TDSRRC_STATE_CELL_FACH:    
        tdsrrcudt_process_cell_fach_state(cmd_ptr); 
        break;

      case TDSRRC_STATE_CELL_DCH:  
        /* The only appropriate state is CELL_DCH. Later on we will have to add
         * code for the other states.
         */   
        tdsrrcudt_process_cell_dch_state(cmd_ptr); 
        break;

      case TDSRRC_STATE_CELL_PCH:
      case TDSRRC_STATE_URA_PCH:
        tdsrrcudt_process_cell_ura_pch_state(cmd_ptr);
        break;

      default:  
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"UDT:Event forwarded in invalid State: %d",rrc_state);
        break;
     }  
  }
} /* tdsrrcudt_procedure_event_handler */


/*===========================================================================

FUNCTION tdsrrcscrr_validate_close_session_req  

DESCRIPTION
  This function validates TDSRRC_CLOSE_SESSION_REQ command received from MM task.

DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcdt_status_e_type: It returns TDSRRCDT_SUCCESS if it is successfully validates 
  the TDSRRC_OPEN_SESSION_REQ command. Otherwise it returns TDSRRCDT_FAILURE_MINOR.

===========================================================================*/

static tdsrrcdt_status_e_type tdsrrcscrr_validate_close_session_req
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{
  tdsrrcdt_status_e_type status = TDSRRCDT_FAILURE_MINOR; 

  boolean domain_open;                  /* Indicates if the domain is open */

   /* Assume that the domain associated with the flow ID is closed
   */
  domain_open = FALSE;

  /* Check to see if a signalling session to the requested domain is open
   */
  switch (cmd_ptr->cmd.close_sess_req.cn_domain_id)
  {
    case RRC_CS_DOMAIN_CN_ID :
      if (tdsrrcdt_int_data.cs_domain_open)
      {
        domain_open = TRUE;
      }
      break;

    case RRC_PS_DOMAIN_CN_ID :
      if (tdsrrcdt_int_data.ps_domain_open)
      {
        domain_open = TRUE;
      }
      break;

    default :
     
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid CN Domain Id  %d",cmd_ptr->cmd.close_sess_req.cn_domain_id);
      break;
  }

#ifdef FEATURE_UE_INITIATED_DORMANCY
  if((cmd_ptr->cmd.close_sess_req.close_session_cause == RRC_UE_INITIATED_DORMANCY)&&
     (TDSRRC_NOT_CAMPED_ON == tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_SCRR))
    )
  {
    /* For UE INITIATED DORMANCY feature, If UE is in OOS, 
    * Then return Close session cnf with failuer cause RRC_CLOSE_SESS_UE_OOS,
    * NAS needs to return the Close status Immediately to upper layer
    */
    tdsrrcscrr_send_close_session_cnf(RRC_CLOSE_SESS_FAILURE_UE_INIT_DORMANCY_UE_IN_OOS, 
                                cmd_ptr->cmd.close_sess_req.cn_domain_id);  
    status = TDSRRCDT_FAILURE_MINOR;
  }
  else
#endif
  {
    /* If the domain is not open, print a message and exit
     */
    if (domain_open == FALSE)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"CN Domain ID not open: %d",cmd_ptr->cmd.close_sess_req.cn_domain_id);
      if (tdsrrcidt_is_idt_buffered(cmd_ptr->cmd.close_sess_req.cn_domain_id)== TRUE)
      {
        if(tdsrrcidt_can_buffered_idt_be_deleted(cmd_ptr->cmd.close_sess_req.cn_domain_id) == TRUE)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deleted IDT for domain %d & sending close cnf to NAS",cmd_ptr->cmd.close_sess_req.cn_domain_id);
    
          if(TDSRRC_NOT_CAMPED_ON == tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_SCRR))
          {
            tdsrrcscrr_send_close_session_cnf(RRC_CLOSE_SESS_UE_OOS, 
                                       cmd_ptr->cmd.close_sess_req.cn_domain_id);  
          }
          else
          {
            tdsrrcscrr_send_close_session_cnf(RRC_CLOSE_SESS_SUCCESS, 
                                       cmd_ptr->cmd.close_sess_req.cn_domain_id);  
          }
    
          status = TDSRRCDT_FAILURE_MINOR;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IDT has triggered CU,Let SCRR be Serialize with IDT ");
          status = TDSRRCDT_SUCCESS;
        }
      }
  #ifdef FEATURE_UE_INITIATED_DORMANCY
      else if(cmd_ptr->cmd.close_sess_req.close_session_cause == RRC_UE_INITIATED_DORMANCY)
      {
        /* For UE INITIATED DORMANCY feature, If CN doain is not open , 
        * Then return Close session cnf with success cause RRC_CLOSE_SESS_SUCCESS_UE_INIT_DORMANCY,
        * NAS needs to return the Close status Immediately to upper layer.
        * This race condition can happen when Close_req is pending in RRC queue & RRC process 
        * SCR which close this CN doamin 
        */
        tdsrrcscrr_send_close_session_cnf(RRC_CLOSE_SESS_SUCCESS_UE_INIT_DORMANCY, 
                                    cmd_ptr->cmd.close_sess_req.cn_domain_id);  
        status = TDSRRCDT_FAILURE_MINOR;
      }
  #endif
      else
      {
        /* If TDSRRC_CLOSE_SESSION_REQ primitive is invalid, just trash it */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid TDSRRC_CLOSE_SESSION_REQ command");
        tdsrrcscrr_send_close_session_cnf(RRC_CLOSE_SESS_FAILURE, 
              cmd_ptr->cmd.close_sess_req.cn_domain_id);
        status = TDSRRCDT_FAILURE_MINOR;
      }
    }
    else
    {
      status = TDSRRCDT_SUCCESS;
    }
  }
  return (status);

} /* tdsrrcscrr_validate_close_session_req */
/*===========================================================================

FUNCTION tdsrrcscrr_delete_close_session_req

DESCRIPTION
  This function deletes the Close Session req data from the static variable
  for the specified CN domain

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcscrr_delete_close_session_req
(
  rrc_cn_domain_identity_e_type cn_domain_id
)
{
  /*
   * Delete the flag indicating whether PS/CS domain open
   */
  switch (cn_domain_id)
  {
    case RRC_CS_DOMAIN_CN_ID :
      tdsrrcscrr_saved_cs_close_session_req.close_sess_req_status = FALSE;
      tdsrrcscrr_saved_cs_close_session_req.mui_status = TDSRRCSCRR_MUI_STATUS_FALSE;
      tdsrrcscrr_saved_cs_close_session_req.rrc_sig_con_rel_status = FALSE;
#if defined(FEATURE_3GPP_FAST_DORMANCY)
      tdsrrcscrr_saved_cs_close_session_req.rrc_proc = TDSRRC_PROCEDURE_NONE;
#endif
    break;

    case RRC_PS_DOMAIN_CN_ID :
      tdsrrcscrr_saved_ps_close_session_req.close_sess_req_status = FALSE;
      tdsrrcscrr_saved_ps_close_session_req.mui_status = TDSRRCSCRR_MUI_STATUS_FALSE;
      tdsrrcscrr_saved_ps_close_session_req.rrc_sig_con_rel_status = FALSE;
#if defined(FEATURE_3GPP_FAST_DORMANCY)
      tdsrrcscrr_saved_ps_close_session_req.rrc_proc = TDSRRC_PROCEDURE_NONE;
#endif
      break;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid CN Domain: %d" ,cn_domain_id);
      break;
  
  }
}/*tdsrrcscrr_delete_close_session_req*/

/*===========================================================================

FUNCTION tdsrrcscrr_send_sig_conn_rel_req

DESCRIPTION
  Copy contents of saved_close_session_req and send the Signalling Conection
  Release request message on UL DCCH for CS/PS domains.

DEPENDENCIES
  None.

RETURN VALUE
  void

===========================================================================*/
static void tdsrrcscrr_send_sig_conn_rel_req
(
  rrc_cn_domain_identity_e_type domain_id
)
{
  tdsrrc_UL_DCCH_Message *msg_ptr;         			        /* Pointer to uplink DCCH message */  
  rlc_lc_id_type ul_lc_id;                        			/* Uplink logical channel ID */
  tdsrrcscrr_close_sess_req_type *save_close_session_req_ptr; 	/* Ptr to close session Req*/
  tdsrrcsend_status_e_type send_status;           				/* send status */
  uint8 msg_mui;                              			    /* MUI for the SCRR */
  
  
  if(domain_id == RRC_CS_DOMAIN_CN_ID)
  {
    save_close_session_req_ptr = &tdsrrcscrr_saved_cs_close_session_req;
  }
  else
  {
    save_close_session_req_ptr = &tdsrrcscrr_saved_ps_close_session_req;
  }
    
  /* Check to see if there is any saved message and send SigConnReq */
  if((save_close_session_req_ptr->close_sess_req_status == TRUE) ||
     (save_close_session_req_ptr->rrc_sig_con_rel_status == TRUE)) 
  {
    /* Determine the logical channel id for the Uplink DCCH */
    ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH,
      DCCH_AM_RADIO_BEARER_ID, UE_MODE_ACKNOWLEDGED_DATA);
      
    if (ul_lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
    {
      /* The logical channel id was not found */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB 2 not found by LCM");
      tdsrrcscrr_delete_close_session_req(domain_id);
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
      TDSRRCSET_MSG_TYPE(msg_ptr->message,
      T_tdsrrc_UL_DCCH_MessageType_signallingConnectionReleaseIndication);
      
      /* Initialize the bit mask for Signalling Connection Release Indication Message */
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.signallingConnectionReleaseIndication);
      
      /* Set the CN Domain ID from the MM primitive */
      switch (domain_id)
      {
        case RRC_CS_DOMAIN_CN_ID :
          tdsrrcsmc_update_ciphering_config_info_for_signalling_release(domain_id);        
          msg_ptr->message.u.signallingConnectionReleaseIndication.cn_DomainIdentity = tdsrrc_CN_DomainIdentity_cs_domain;
          break;

        case RRC_PS_DOMAIN_CN_ID :
          tdsrrcsmc_update_ciphering_config_info_for_signalling_release(domain_id);               
          msg_ptr->message.u.signallingConnectionReleaseIndication.cn_DomainIdentity = tdsrrc_CN_DomainIdentity_ps_domain;
          break;

        default :
          /* We should never get here because the Domain ID was already validated above */
          ERR_FATAL("Invalid Domain ID %d",domain_id,0,0);
          break;
      }

      /* Send the Signalling Connection Release Request Message on the Uplink DCCH */
      send_status = tdsrrcsend_send_ul_sdu_with_mui(TDSRRC_PROCEDURE_SCRR,msg_ptr,ul_lc_id,
                                                 TDSRRCSEND_L2ACK_REQUIRED, &msg_mui);
      if (send_status == TDSRRCSEND_SUCCESS)
      {
        tdsrrcscrr_substate = TDSRRCSCRR_WAIT_FOR_L2ACK;
		save_close_session_req_ptr->mui = msg_mui;
        /* update the mui_status cause and also the flag to indicate that close_sess_req
         * or sig_con_rel_req has been already sent */
        if(save_close_session_req_ptr->close_sess_req_status == TRUE)
        {
          save_close_session_req_ptr->mui_status = TDSRRCSCRR_MUI_CLOSE_SESSION_CNF;
          save_close_session_req_ptr->close_sess_req_status = FALSE;            
        }
        else
        {
          save_close_session_req_ptr->mui_status = TDSRRCSCRR_MUI_SIG_CON_REL_CNF;
          save_close_session_req_ptr->rrc_sig_con_rel_status = FALSE;
        }

        TDSRRC_MSG2(MSG_LEGACY_HIGH,"SigConRel for domain %d cause %d ",domain_id,save_close_session_req_ptr->mui_status);
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Error %d sending SCRR Message",send_status);
        tdsrrcscrr_delete_close_session_req(domain_id);
        tdsrrcscrr_substate = TDSRRCSCRR_INITIAL;
      }
    }
  }
}/*tdsrrcscrr_send_sig_conn_rel_req*/

/*===========================================================================

FUNCTION RRCSCRR_PROCESS_SIG_CONN_REL_REQ

DESCRIPTION
  Check the current RRC state and send the sig conn req.If CU is active
  wair for CU to complete before sending the request

DEPENDENCIES
  None.

RETURN VALUE
  void

===========================================================================*/
void tdsrrcscrr_process_sig_conn_rel_req
(
  rrc_cn_domain_identity_e_type domain_id
)
{
  switch(tdsrrc_get_state())
  {
	case TDSRRC_STATE_URA_PCH:
	case TDSRRC_STATE_CELL_PCH:
	case TDSRRC_STATE_CELL_FACH:
	  /* This queries whether a CELL Update procedure is active.If Cell Update
           * is active, it registers for the complete and enters the substate
           * TDSRRCIDT_WAIT_FOR_CELL_UPDATE_COMP_CNF */ 
	  if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status(TDSRRC_PROCEDURE_SCRR) )
	  {
		/* Wait until Cell update procedure is completed */
		tdsrrcscrr_substate = TDSRRCSCRR_WAIT_FOR_CELL_UPDATE_COMP_CNF;  
	  }
	  else /* Cell Update procedure is inactive */
	  {
            /* Build and send the Signalling Connection Release Request to Send Chain */
		tdsrrcscrr_send_sig_conn_rel_req(domain_id);
	  }
	  break;

	case TDSRRC_STATE_CELL_DCH:
	  /* Build and send the Open Session Request to Send Chain */
	  tdsrrcscrr_send_sig_conn_rel_req(domain_id);
	  break;

	default:
	  TDSRRC_MSG1(MSG_LEGACY_HIGH,"SCRR:Invalid Event in rrc state: %d",tdsrrc_get_state());
	  break;  
  }  
}

/*===========================================================================

FUNCTION tdsrrcscrr_save_sig_release_req

DESCRIPTION
  This function copies into saved_close_session_req static variable
  the contents of message received in cmd_ptr

DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcdt_status_e_type.

===========================================================================*/
static tdsrrcdt_status_e_type tdsrrcscrr_save_sig_release_req
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  tdsrrcdt_status_e_type save_status = TDSRRCDT_FAILURE_MINOR;
  
  /* check and see if the close session request is for
   * CS or PS domain
   */
  switch (cmd_ptr->cmd.rrc_sig_conn_req.cn_domain_id)
  {
    case RRC_CS_DOMAIN_CN_ID :
      if (( tdsrrcscrr_saved_cs_close_session_req.close_sess_req_status == FALSE )&&
          ( tdsrrcscrr_saved_cs_close_session_req.rrc_sig_con_rel_status == FALSE) &&
          (tdsrrcscrr_saved_cs_close_session_req.mui_status == TDSRRCSCRR_MUI_STATUS_FALSE)
         )
      {
        tdsrrcscrr_saved_cs_close_session_req.rrc_sig_con_rel_status = TRUE;
        /* RCE can send sig_conn_rel for cs domain */
        tdsrrcscrr_saved_cs_close_session_req.rrc_proc = 
          cmd_ptr->cmd.rrc_sig_conn_req.rrc_proc;
        save_status = TDSRRCDT_SUCCESS;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Multiple Close Sess not allowed: %d",cmd_ptr->cmd.close_sess_req.cn_domain_id);    
      }
    
      break;
    
    case RRC_PS_DOMAIN_CN_ID :
      if (( tdsrrcscrr_saved_ps_close_session_req.close_sess_req_status == FALSE ) &&
          ( tdsrrcscrr_saved_ps_close_session_req.rrc_sig_con_rel_status == FALSE ) &&
          ( tdsrrcscrr_saved_ps_close_session_req.mui_status == TDSRRCSCRR_MUI_STATUS_FALSE)
         )
      {
        tdsrrcscrr_saved_ps_close_session_req.rrc_sig_con_rel_status = TRUE;
        tdsrrcscrr_saved_ps_close_session_req.rrc_proc = 
          cmd_ptr->cmd.rrc_sig_conn_req.rrc_proc;
        save_status = TDSRRCDT_SUCCESS;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Multiple Close Sess not allowed: %d",cmd_ptr->cmd.close_sess_req.cn_domain_id);
      }
      break;
    
    default:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid Domain for Close Session Req");
      break;
  }

  return(save_status);
}/*tdsrrcscrr_save_sig_release_req*/
/*===========================================================================

FUNCTION tdsrrcscrr_save_close_session_req

DESCRIPTION
  This function copies into saved_close_session_req static variable
  the contents of message received in cmd_ptr

DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcdt_status_e_type.

===========================================================================*/
static tdsrrcdt_status_e_type tdsrrcscrr_save_close_session_req
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  tdsrrcdt_status_e_type save_status = TDSRRCDT_FAILURE_MINOR;
  
  /* check and see if the close session request is for
   * CS or PS domain
   */
  switch (cmd_ptr->cmd.close_sess_req.cn_domain_id)
  {
    case RRC_CS_DOMAIN_CN_ID :
      if (( tdsrrcscrr_saved_cs_close_session_req.close_sess_req_status == FALSE ) &&
          ( tdsrrcscrr_saved_cs_close_session_req.rrc_sig_con_rel_status == FALSE ) &&
          (tdsrrcscrr_saved_cs_close_session_req.mui_status == TDSRRCSCRR_MUI_STATUS_FALSE)
         )
      {
        tdsrrcscrr_saved_cs_close_session_req.close_sess_req_status = TRUE;
        save_status = TDSRRCDT_SUCCESS;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Multiple Close Sess not allowed: %d",cmd_ptr->cmd.close_sess_req.cn_domain_id);    
      }
    
      break;
    
    case RRC_PS_DOMAIN_CN_ID :
      if (( tdsrrcscrr_saved_ps_close_session_req.close_sess_req_status == FALSE ) &&
          ( tdsrrcscrr_saved_ps_close_session_req.rrc_sig_con_rel_status == FALSE ) &&
          (tdsrrcscrr_saved_ps_close_session_req.mui_status == TDSRRCSCRR_MUI_STATUS_FALSE)
         )
      {
        tdsrrcscrr_saved_ps_close_session_req.close_sess_req_status = TRUE;
        save_status = TDSRRCDT_SUCCESS;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Multiple Close Sess not allowed: %d",cmd_ptr->cmd.close_sess_req.cn_domain_id);
      }
      break;
    
    default:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid Domain for Close Session Req");
      break;
  }

  return(save_status);
}/*tdsrrcscrr_save_close_session_req*/

/*===========================================================================

FUNCTION tdsrrcscrr_send_close_session_cnf  

DESCRIPTION
  This function builds the RRC_CLOSE_SESSION_CNF command with parameters and 
  posts the same to the MM task.
DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcdt_status_e_type: It returns TDSRRCDT_SUCCESS if it is successfully sends 
  the RRC_CLOSE_SESSION_CNF command. Otherwise it returns TDSRRCDT_FAILURE_MAJOR.

===========================================================================*/

void tdsrrcscrr_send_close_session_cnf
(
  rrc_close_session_status_e_type     session_status,  /* Success or failure
                                                          status*/
  rrc_cn_domain_identity_e_type       cn_domain_id     /* CN Domain Id */

)
{
  mm_cmd_type *cmd_ptr;                      /* Pointer to the MM Command */


#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"CSFB_HACK: tdsrrc_csfb_extended_status = %d, close session for domain %d, PS domain status %d",
            tdsrrc_csfb_extended_status,cn_domain_id,tdsrrcdt_int_data.ps_domain_open);
  if((RRC_CLOSE_SESS_SUCCESS == session_status) && (tdsrrc_csfb_extended_status == CSFB_STARTED) &&
      (cn_domain_id == RRC_CS_DOMAIN_CN_ID))
  {
    tdsrrcrce_post_internal_csfb_abort_cmd();
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"CSFB_HACK: tdsrrc_csfb_extended_status = %d",tdsrrc_csfb_extended_status);
    return;
  }
#endif


  /* Allocate memory for RRC_CLOSE_SESSION_CNF command */
  if( (cmd_ptr =  mm_tdsrrc_get_cmd_buf(RRC_CLOSE_SESSION_CNF)) != NULL)
  {
    /* Fill in parameters fro RRC_CLOSE_SESSION_CNF command */
    cmd_ptr->cmd.rrc_close_session_cnf.cn_domain_id= cn_domain_id ;

    tdsrrcsmc_update_sync_ind_info_for_signalling_release(cn_domain_id);
    cmd_ptr->cmd.rrc_close_session_cnf.status = session_status;

#ifdef FEATURE_DUAL_SIM
    cmd_ptr->cmd.rrc_close_session_cnf.as_id = tdsrrc_get_as_id();
#endif

    /* Sends the command to MM */
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(cmd_ptr);
    }
    mm_put_cmd(cmd_ptr);

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Sent RRC_CLOSE_SESSION_CNF to MM with status =%d,domain id =%d",session_status,cn_domain_id);

  }    
  else   /* RRC Task failed to allocate memory */
  {
    ERR_FATAL("Unable to allocate memory for MM Cmd: %d",RRC_CLOSE_SESSION_CNF,0,0);
  }     

} /* tdsrrcscrr_send_close_session_cnf */


/*===========================================================================

FUNCTION tdsrrcscrr_send_sig_conn_release_cnf  

DESCRIPTION
  This function builds the TDSRRC_SIG_CONN_RELEASE_CNF command with parameters and 
  posts the same to the MM task.
DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcdt_status_e_type: It returns TDSRRCDT_SUCCESS if it is successfully sends 
  the TDSRRC_SIG_CONN_RELEASE_CNF command. Otherwise it returns TDSRRCDT_FAILURE_MAJOR.

===========================================================================*/
void tdsrrcscrr_send_sig_conn_release_cnf
(
  uecomdef_status_e_type              status  /* Success or failure status*/
  , tdsrrc_proc_e_type                     rrc_proc  /* Dest rrc proc */
)
{
  tdsrrc_cmd_type *cmd_ptr;                      /* Pointer to the Command */


  /* Allocate memory for RRC_CLOSE_SESSION_CNF command */
  if( (cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {
    cmd_ptr->cmd_hdr.cmd_id = TDSRRC_SIG_CONN_RELEASE_CNF;

    /* Fill in parameters for TDSRRC_SIG_CONN_RELEASE_CNF command */
    cmd_ptr->cmd.rrc_sig_conn_cnf.status = status;
    cmd_ptr->cmd.rrc_sig_conn_cnf.rrc_proc = rrc_proc;
   
    tdsrrc_put_int_cmd(cmd_ptr);                    /* Sends the command to MM */

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sent TDSRRC_SIG_CONN_RELEASE_CNF to Proc:%d",rrc_proc);

  }    
  else   /* RRC Task failed to allocate memory */
  {
    ERR_FATAL("Unable to allocate memory for MM Cmd: %d",TDSRRC_SIG_CONN_RELEASE_CNF,0,0);
  }     

} /* tdsrrcscrr_send_sig_conn_release_cnf */


/*===========================================================================

FUNCTION tdsrrcscrr_process_scrr_initial_substate

DESCRIPTION
  This function processes all events that are  dispatched
  to Signalling Connection Release Request procedure in TDSRRCSCRR_INITIAL
  substate

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

static void tdsrrcscrr_process_scrr_initial_substate
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  rrc_cn_domain_identity_e_type domain_id;

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_CLOSE_SESSION_REQ:
    case TDSRRC_SIG_CONN_RELEASE_REQ:
       
      if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_CLOSE_SESSION_REQ)
      {
        /* Assume that the domain associated with the flow ID is closed */
        if(TDSRRCDT_SUCCESS != tdsrrcscrr_validate_close_session_req(cmd_ptr))
        {
          /*Moved sending Close Session Cnf in tdsrrcscrr_validate_close_session_req function*/
          return;
        }
      
        if(TDSRRCDT_SUCCESS != tdsrrcscrr_save_close_session_req(cmd_ptr))
        {
          /* If there are multiple requests,then just trash it */
          /*lint -e666*/
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Failed to Save Close Sess Req in: %d,%d",tdsrrc_get_state(),tdsrrcscrr_substate);
          /*lint +e666*/
  
          tdsrrcscrr_send_close_session_cnf(RRC_CLOSE_SESS_FAILURE, 
            cmd_ptr->cmd.close_sess_req.cn_domain_id);
          return;
        }
        domain_id = cmd_ptr->cmd.close_sess_req.cn_domain_id;
      }
      else if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_SIG_CONN_RELEASE_REQ)
      {
        boolean domain_is_open = FALSE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received TDSRRC_SIG_CONN_RELEASE_REQ");
        domain_id = cmd_ptr->cmd.rrc_sig_conn_req.cn_domain_id;
        if(domain_id == RRC_CS_DOMAIN_CN_ID)
        {
          domain_is_open = tdsrrcdt_is_cs_domain_open();
        }
        else if(domain_id == RRC_PS_DOMAIN_CN_ID)
        {
          domain_is_open = tdsrrcdt_is_ps_domain_open();
        }
        else
        {
          domain_is_open = TRUE;
        }

        if(domain_is_open == FALSE)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"The domain %d is already closed",domain_id);
#if defined(FEATURE_3GPP_FAST_DORMANCY)
          tdsrrcscrr_send_sig_conn_release_cnf(
              SUCCESS, 
              cmd_ptr->cmd.rrc_sig_conn_req.rrc_proc);
#else
          tdsrrcscrr_send_sig_conn_release_cnf(SUCCESS);
#endif
          return;
        }
        /* No need to validate this command */
        if(TDSRRCDT_SUCCESS != tdsrrcscrr_save_sig_release_req(cmd_ptr))
        {
          /* If there are multiple requtes, just trash it */
          /*lint -e666*/
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Failed to Save Sig Rel Req in: %d,%d",tdsrrc_get_state(),tdsrrcscrr_substate);
          /*lint +e666*/
  
#if defined(FEATURE_3GPP_FAST_DORMANCY)
          tdsrrcscrr_send_sig_conn_release_cnf(
                    FAILURE,
                    cmd_ptr->cmd.rrc_sig_conn_req.rrc_proc);
#else
          tdsrrcscrr_send_sig_conn_release_cnf(FAILURE);
#endif            
          return;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid domain id");
        return;
      }

      /* Send the SCRR only if an IDT for the same domain is not currently
      active.If an IDT is active,then wait for IDT to complete and then send
      the SigConnRelInd out.This is to avoid a race condition where IDT reaches
      the network after SCRR while the reverse was intended */
      if(FALSE == tdsrrcidt_is_idt_active(domain_id))
      {
        tdsrrcscrr_process_sig_conn_rel_req(domain_id);
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Defer processing SCRR since IDT is active.domain:%d",domain_id);
      }
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d for SCRR in substate:%d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcscrr_substate);
      break;
  }/*end of switch stmt*/
} /* tdsrrcscrr_process_scrr_initial_substate */
/*===========================================================================

FUNCTION tdsrrcscrr_process_scrr_wait_for_cell_update_comp_cnf_substate

DESCRIPTION
  This function processes all events that are  dispatched
  to Signalling Connection Release Request procedure in 
  TDSRRCSCRR_WAIT_FOR_CELL_UPDATE_COMP_CNF substate

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

static void tdsrrcscrr_process_scrr_wait_for_cell_update_comp_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{
  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
      /* Check to see if there are any saved SCRR messages waiting to be transmitted.
      Make sure to check both the domains and send out the message on a per domain 
      basis.Check the MUI status also because re-transmission would take a diff
      path */
      if(((tdsrrcscrr_saved_cs_close_session_req.close_sess_req_status == TRUE)||
          (tdsrrcscrr_saved_cs_close_session_req.rrc_sig_con_rel_status == TRUE)) &&
         (tdsrrcscrr_saved_cs_close_session_req.mui_status == TDSRRCSCRR_MUI_STATUS_FALSE))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending SCRR for CS domain after Cu_Complete_Cnf");
        tdsrrcscrr_send_sig_conn_rel_req(RRC_CS_DOMAIN_CN_ID);
      }
      
      if(((tdsrrcscrr_saved_ps_close_session_req.close_sess_req_status == TRUE)||
          (tdsrrcscrr_saved_ps_close_session_req.rrc_sig_con_rel_status == TRUE)) &&
         (tdsrrcscrr_saved_ps_close_session_req.mui_status == TDSRRCSCRR_MUI_STATUS_FALSE))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending SCRR for PS domain after Cu_Complete_Cnf");
        tdsrrcscrr_send_sig_conn_rel_req(RRC_PS_DOMAIN_CN_ID);
      }
      break;

    /* if we get a close session request, it can be for a different cn domain.
     * save the request
     */
    case TDSRRC_CLOSE_SESSION_REQ:
    case TDSRRC_SIG_CONN_RELEASE_REQ:
      if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_CLOSE_SESSION_REQ)
      {
        if(TDSRRCDT_SUCCESS != tdsrrcscrr_validate_close_session_req(cmd_ptr))
        {
          /*Moved sending Close Session Cnf in tdsrrcscrr_validate_close_session_req function*/
          return;
        }
      
        if(TDSRRCDT_SUCCESS != tdsrrcscrr_save_close_session_req(cmd_ptr))
        {
          /* If TDSRRC_CLOSE_SESSION_REQ primitive is invalid, just trash it */
          /*lint -e666*/
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Failed to Save Close Sess Req in: %d,%d",tdsrrc_get_state(),tdsrrcscrr_substate);
          /*lint +e666*/
  
          tdsrrcscrr_send_close_session_cnf(RRC_CLOSE_SESS_FAILURE, 
            cmd_ptr->cmd.close_sess_req.cn_domain_id);
          return;
        }
      }
      if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_SIG_CONN_RELEASE_REQ)
      {
        /* No need to validate this command */
        if(TDSRRCDT_SUCCESS != tdsrrcscrr_save_sig_release_req(cmd_ptr))
        {
          /* If there are multiple requtes, just trash it */
          /*lint -e666*/
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Failed to Save Sig Rel Req in: %d,%d",tdsrrc_get_state(),tdsrrcscrr_substate);
          /*lint +e666*/
#if defined(FEATURE_3GPP_FAST_DORMANCY)
          tdsrrcscrr_send_sig_conn_release_cnf(
                    FAILURE,
                    cmd_ptr->cmd.rrc_sig_conn_req.rrc_proc);
#else
          tdsrrcscrr_send_sig_conn_release_cnf(FAILURE);
#endif
          return;
        }
      }
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"SCRR:Invalid Event: %d in substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcscrr_substate);
      break;
  }  
} /* tdsrrcscrr_process_scrr_wait_for_cell_update_comp_cnf_substate */

/*===========================================================================

FUNCTION tdsrrcscrr_process_scrr_wait_for_l2_ack_substate

DESCRIPTION
  This function processes all events that are  dispatched
  to Signalling Connection Release Request procedure in 
  RRCSCRR_WAIT_FOR_L2_ACK substate

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

static void tdsrrcscrr_process_scrr_wait_for_l2_ack_substate
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{
  rrc_cn_domain_identity_e_type local_domain_id;

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_DOWNLINK_L2ACK_IND:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"SCRR L2-ACK status %d,mui %d",cmd_ptr->cmd.downlink_l2ack_ind.status,cmd_ptr->cmd.downlink_l2ack_ind.mui);

      if((tdsrrcscrr_saved_cs_close_session_req.mui_status != TDSRRCSCRR_MUI_STATUS_FALSE) && 
         (cmd_ptr->cmd.downlink_l2ack_ind.mui == tdsrrcscrr_saved_cs_close_session_req.mui))
      {
        if(tdsrrcscrr_saved_cs_close_session_req.mui_status == TDSRRCSCRR_MUI_CLOSE_SESSION_CNF)
        {
          //send close-session-cnf status based on l2-ack status
          if(SUCCESS == cmd_ptr->cmd.downlink_l2ack_ind.status)
          {
            tdsrrcscrr_send_close_session_cnf(RRC_CLOSE_SESS_SUCCESS,RRC_CS_DOMAIN_CN_ID);
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"CS DOMAIN CLOSE SESSION CNF WITH SUCCESS - DELETE ALL SAVED UDTs");
            tdsrrcudt_delete_all_saved_data_req(RRC_CS_DOMAIN_CN_ID);
            tdsrrcdt_int_data.cs_domain_open = FALSE;
            if(tdsrrcudt_any_saved_data_req_for_cn_domain(RRC_PS_DOMAIN_CN_ID) == FALSE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"There is no saved UDT for PS domain,set udt substate to init");
              tdsrrcudt_substate = TDSRRCUDT_INITIAL;
            }
          }
          else
          {
            tdsrrcscrr_saved_cs_close_session_req.close_sess_req_status =TRUE;
            tdsrrcscrr_send_sig_conn_rel_req(RRC_CS_DOMAIN_CN_ID);
            break;
          }
        }
        else if(tdsrrcscrr_saved_cs_close_session_req.mui_status == TDSRRCSCRR_MUI_SIG_CON_REL_CNF)
        {
          //send sig-conn-release-cnf status based on l2-ack status
          if(SUCCESS == cmd_ptr->cmd.downlink_l2ack_ind.status)
          {
#if defined(FEATURE_3GPP_FAST_DORMANCY)
            tdsrrcscrr_send_sig_conn_release_cnf(
                        SUCCESS, 
                        tdsrrcscrr_saved_cs_close_session_req.rrc_proc);
#else
            tdsrrcscrr_send_sig_conn_release_cnf(SUCCESS);
#endif
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"CS DOMAIN SCRR CNF WITH SUCCESS - DELETE ALL SAVED UDTs");
            tdsrrcudt_delete_all_saved_data_req(RRC_CS_DOMAIN_CN_ID);
            if(tdsrrcudt_any_saved_data_req_for_cn_domain(RRC_PS_DOMAIN_CN_ID) == FALSE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"There is no saved UDT for PS domain,set udt substate to init");
              tdsrrcudt_substate = TDSRRCUDT_INITIAL;
            }
            tdsrrcdt_int_data.cs_domain_open = FALSE;
          }
          else
          {
            tdsrrcscrr_saved_cs_close_session_req.rrc_sig_con_rel_status =TRUE;
            tdsrrcscrr_send_sig_conn_rel_req(RRC_CS_DOMAIN_CN_ID);
            break;
          }
        }
       
        if(tdsrrcscrr_saved_ps_close_session_req.mui_status != TDSRRCSCRR_MUI_STATUS_FALSE)
        {
          tdsrrcscrr_substate = TDSRRCSCRR_WAIT_FOR_L2ACK; 
        }
        else
        {
          tdsrrcscrr_substate = TDSRRCSCRR_INITIAL; 
        }

        tdsrrcscrr_delete_close_session_req(RRC_CS_DOMAIN_CN_ID);
          
      }
      else if((tdsrrcscrr_saved_ps_close_session_req.mui_status != TDSRRCSCRR_MUI_STATUS_FALSE) && 
              (cmd_ptr->cmd.downlink_l2ack_ind.mui == tdsrrcscrr_saved_ps_close_session_req.mui))
      {
        //check if the sig_conn_rel_ind was for close_session_req or sig_con_rel_req
        //and send the appropriate RSP to RCE
        if(tdsrrcscrr_saved_ps_close_session_req.mui_status == TDSRRCSCRR_MUI_CLOSE_SESSION_CNF)
        {
          //send cnf based on L2-ACK status
          if(SUCCESS == cmd_ptr->cmd.downlink_l2ack_ind.status)
          {
            tdsrrcscrr_send_close_session_cnf(RRC_CLOSE_SESS_SUCCESS, RRC_PS_DOMAIN_CN_ID);
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"PS DOMAIN CLOSE SESSION CNF WITH SUCCESS - DELETE ALL SAVED UDTs");
            tdsrrcudt_delete_all_saved_data_req(RRC_PS_DOMAIN_CN_ID);
            tdsrrcdt_int_data.ps_domain_open = FALSE;
            if(tdsrrcudt_any_saved_data_req_for_cn_domain(RRC_CS_DOMAIN_CN_ID) == FALSE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"There is no saved UDT for CS domain,set udt substate to init");
              tdsrrcudt_substate = TDSRRCUDT_INITIAL;
            }
          }
          else
          {
            tdsrrcscrr_saved_ps_close_session_req.close_sess_req_status =TRUE;
            tdsrrcscrr_send_sig_conn_rel_req(RRC_PS_DOMAIN_CN_ID);
            break;      
          }
        }
        else if(tdsrrcscrr_saved_ps_close_session_req.mui_status == TDSRRCSCRR_MUI_SIG_CON_REL_CNF)
        {
          //send cnf based on L2-ACK status
          if(SUCCESS == cmd_ptr->cmd.downlink_l2ack_ind.status)
          {
#if defined(FEATURE_3GPP_FAST_DORMANCY)
            tdsrrcscrr_send_sig_conn_release_cnf(
                        SUCCESS,
                        tdsrrcscrr_saved_ps_close_session_req.rrc_proc);
#else
            tdsrrcscrr_send_sig_conn_release_cnf(SUCCESS);
#endif
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"PS DOMAIN SCRR CNF WITH SUCCESS - DELETE ALL SAVED UDTs");
            tdsrrcudt_delete_all_saved_data_req(RRC_PS_DOMAIN_CN_ID);
            tdsrrcdt_int_data.ps_domain_open = FALSE;
            if(tdsrrcudt_any_saved_data_req_for_cn_domain(RRC_CS_DOMAIN_CN_ID) == FALSE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"There is no saved UDT for CS domain,set udt substate to init");
              tdsrrcudt_substate = TDSRRCUDT_INITIAL;
            }


          }
          else
          {
            tdsrrcscrr_saved_ps_close_session_req.rrc_sig_con_rel_status =TRUE;
            tdsrrcscrr_send_sig_conn_rel_req(RRC_PS_DOMAIN_CN_ID);
            break;
          }
        }

        if(tdsrrcscrr_saved_cs_close_session_req.mui_status != TDSRRCSCRR_MUI_STATUS_FALSE)
        {
          tdsrrcscrr_substate = TDSRRCSCRR_WAIT_FOR_L2ACK; 
        }
        else
        {
          tdsrrcscrr_substate = TDSRRCSCRR_INITIAL; 
        }
        
        tdsrrcscrr_delete_close_session_req(RRC_PS_DOMAIN_CN_ID);
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid L2 ACK is rcvd");
      }
      break;

    /* if we get a close session request, it can be for a different cn domain.
     * save the request
     */
    case TDSRRC_CLOSE_SESSION_REQ:
    case TDSRRC_SIG_CONN_RELEASE_REQ:
      if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_CLOSE_SESSION_REQ)
      {
        /* Copy the domain id */
        local_domain_id = cmd_ptr->cmd.close_sess_req.cn_domain_id;

        if(TDSRRCDT_SUCCESS != tdsrrcscrr_validate_close_session_req(cmd_ptr))
        {
          /*Moved sending Close Session Cnf in tdsrrcscrr_validate_close_session_req function*/
          return;
        }
      
        if(TDSRRCDT_SUCCESS != tdsrrcscrr_save_close_session_req(cmd_ptr))
        {
          /* If TDSRRC_CLOSE_SESSION_REQ primitive is invalid, just trash it */
          /*lint -e666*/
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Failed to Save Close Sess Req in: %d,%d",tdsrrc_get_state(),tdsrrcscrr_substate);
          /*lint +e666*/
  
          tdsrrcscrr_send_close_session_cnf(RRC_CLOSE_SESS_FAILURE,cmd_ptr->cmd.close_sess_req.cn_domain_id);
          return;
        }
      }
      else if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_SIG_CONN_RELEASE_REQ)
      {
        /* Copy the domain id */
        local_domain_id = cmd_ptr->cmd.rrc_sig_conn_req.cn_domain_id;

        /* No need to validate this command */
        if(TDSRRCDT_SUCCESS != tdsrrcscrr_save_sig_release_req(cmd_ptr))
        {
          /* If there are multiple requtes, just trash it */
          /*lint -e666*/
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Failed to Save Sig Rel Req in: %d,%d",tdsrrc_get_state(),tdsrrcscrr_substate);
          /*lint +e666*/
#if defined(FEATURE_3GPP_FAST_DORMANCY)
          tdsrrcscrr_send_sig_conn_release_cnf(
                    FAILURE,
                     cmd_ptr->cmd.rrc_sig_conn_req.rrc_proc);
#else
          tdsrrcscrr_send_sig_conn_release_cnf(FAILURE);
#endif
          return;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid command");
        return;
      }

      /* Send the SCRR only if an IDT for the same domain is not currently
      active.If an IDT is active,then wait for IDT to complete and then send 
      the SigConnRelInd out.This is to avoid a race condition where IDT reaches
      the network after SCRR while the reverse was intended */
      if(FALSE == tdsrrcidt_is_idt_active(local_domain_id))
      {
        tdsrrcscrr_send_sig_conn_rel_req(local_domain_id);
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Defer processing SCRR since IDT is active.domain:%d",local_domain_id);
      }
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"SCRR:Invalid Event: %d in substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcscrr_substate);
      break;
  }  
} /* tdsrrcscrr_process_scrr_wait_for_l2_ack_substate */

/*===========================================================================

FUNCTION tdsrrcscrr_is_cn_domain_open

DESCRIPTION
  This function will return the status of CN domain whether its open or closed
  It also checks to see if signalling connection relaese or
  close session request is in progress. This function is used by DDT
  to check whether the domain is open.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE : If the CN Domain is open.
  FALSE: If the CN Domain is closed.

===========================================================================*/
uint8 tdsrrcscrr_is_cn_domain_open(rrc_cn_domain_identity_e_type cn_domain)
{
  uint8 is_cn_domain_open = FALSE;  
  if(cn_domain == RRC_CS_DOMAIN_CN_ID)
  {
    if(tdsrrcdt_int_data.cs_domain_open == TRUE)
    {
      /*Checking if signalling Release in progress*/
      if((tdsrrcscrr_saved_cs_close_session_req.mui_status == TDSRRCSCRR_MUI_SIG_CON_REL_CNF)||
         (tdsrrcscrr_saved_cs_close_session_req.mui_status == TDSRRCSCRR_MUI_CLOSE_SESSION_CNF))
      {
        /*Since DT has sent Signalling connection Release Request to UTRAN & waiting for the
            L2-ACK, So discard the DDT message in this sall window*/
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Signalling connection Release is in progress for cn domain =%d",cn_domain);
        is_cn_domain_open = FALSE;
      }
      else
      {
        is_cn_domain_open = TRUE;
      }
    }
    else
    {/*This CN domain is closed*/
      is_cn_domain_open = FALSE;
    }
  }
  else if(cn_domain == RRC_PS_DOMAIN_CN_ID)
  {
    if(tdsrrcdt_int_data.ps_domain_open == TRUE)
    {
      /*Checking if signalling Release in progress*/
      if((tdsrrcscrr_saved_ps_close_session_req.mui_status == TDSRRCSCRR_MUI_SIG_CON_REL_CNF)||
         (tdsrrcscrr_saved_ps_close_session_req.mui_status == TDSRRCSCRR_MUI_CLOSE_SESSION_CNF))
      {
        /*Since DT has sent Signalling connection Release Request to UTRAN & waiting for the
        L2-ACK, So discard the DDT message in this sall window*/
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Signalling connection Release is in progress for cn domain =%d",cn_domain);
        is_cn_domain_open = FALSE;
      }
      else
      {
        is_cn_domain_open = TRUE;
      }
    }
    else
    {/*This CN domain is closed*/
      is_cn_domain_open = FALSE;
    }
  }
  else
  {/*This will not exicute since this function is called with valid CN domain*/
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid CN Domain = %d",cn_domain);
  }
  return is_cn_domain_open;
}
/*===========================================================================

FUNCTION tdsrrcscrr_process_cell_fach_state

DESCRIPTION
  This function processes all events that are dispatched to Signalling 
  Connection Release Request procedure in CELL_FACH state of Connected mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcscrr_process_cell_fach_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  switch (tdsrrcscrr_substate)
  {
    case TDSRRCSCRR_INITIAL:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_CLOSE_SESSION_REQ:
        case TDSRRC_SIG_CONN_RELEASE_REQ:
          tdsrrcscrr_process_scrr_initial_substate(cmd_ptr);
          break;
        
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"SCRR:Invalid event in Substate: %d",tdsrrcscrr_substate);
         break;
      }
      break;

    case TDSRRCSCRR_WAIT_FOR_CELL_UPDATE_COMP_CNF: 
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        /* we can either get a complete confirmation or get a close session request
         * for a different CN domain.  Process both in wait for cell update comp cnf
         */
        case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
        case TDSRRC_CLOSE_SESSION_REQ:
        case TDSRRC_SIG_CONN_RELEASE_REQ:
          tdsrrcscrr_process_scrr_wait_for_cell_update_comp_cnf_substate(cmd_ptr);
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"SCRR:Invalid event in Substate: %d",tdsrrcscrr_substate);

          break;
      }
      break;

    case TDSRRCSCRR_WAIT_FOR_L2ACK: 
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        /* we can either get a complete confirmation or get a close session request
         * for a different CN domain.  Process both in wait for cell update comp cnf
         */
        case TDSRRC_DOWNLINK_L2ACK_IND:
        case TDSRRC_CLOSE_SESSION_REQ:
        case TDSRRC_SIG_CONN_RELEASE_REQ:
          tdsrrcscrr_process_scrr_wait_for_l2_ack_substate(cmd_ptr);
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"SCRR:Invalid event in Substate: %d",tdsrrcscrr_substate);

          break;
      }
      break;

    default:   
      /*lint -e793*/        
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"SCRR:Invalid substate: %d  in CELL FACH",tdsrrcscrr_substate);
      /*lint +e793*/
      break;
  }  
}/* tdsrrcscrr_process_cell_fach_state */
/*===========================================================================

FUNCTION tdsrrcscrr_process_cell_dch_state

DESCRIPTION
  This function processes all events that are dispatched to Signalling
  Connection Release Request procedure in CELL_DCH state of Connected mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcscrr_process_cell_dch_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  switch (tdsrrcscrr_substate)
  {
    case TDSRRCSCRR_INITIAL:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_CLOSE_SESSION_REQ:
        case TDSRRC_SIG_CONN_RELEASE_REQ:
          tdsrrcscrr_process_scrr_initial_substate(cmd_ptr);
          break;
        
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"SCRR:Invalid event in Substate: %d",tdsrrcscrr_substate);
          break;
      }
      break;

    case TDSRRCSCRR_WAIT_FOR_CELL_UPDATE_COMP_CNF: 
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        /* we can either get a complete confirmation or get a close session request
         * for a different CN domain.  Process both in wait for cell update comp cnf
         */
        case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
        case TDSRRC_CLOSE_SESSION_REQ:
        case TDSRRC_SIG_CONN_RELEASE_REQ:
          tdsrrcscrr_process_scrr_wait_for_cell_update_comp_cnf_substate(cmd_ptr);
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"SCRR:Invalid event in Substate: %d",tdsrrcscrr_substate);

          break;
      }
      break;

    case TDSRRCSCRR_WAIT_FOR_L2ACK: 
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        /* we can either get a complete confirmation or get a close session request
         * for a different CN domain.  Process both in wait for cell update comp cnf
         */
        case TDSRRC_DOWNLINK_L2ACK_IND:
        case TDSRRC_CLOSE_SESSION_REQ:
        case TDSRRC_SIG_CONN_RELEASE_REQ:
          tdsrrcscrr_process_scrr_wait_for_l2_ack_substate(cmd_ptr);
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"SCRR:Invalid event in Substate: %d",tdsrrcscrr_substate);

          break;
      }
      break;

    default:           
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"SCRR:Invalid substate: %d in CELL_DCH",tdsrrcscrr_substate);
      break;
  }  
}/* tdsrrcscrr_process_cell_dch_state */

/*===========================================================================

FUNCTION tdsrrcscrr_process_cell_ura_pch_state

DESCRIPTION
  This function processes all events that are dispatched to Signalling 
  Connection Release Request procedure in either in CELL_PCH or URA_PCH
  state of Connected mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcscrr_process_cell_ura_pch_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  switch (tdsrrcscrr_substate)
  {
    case TDSRRCSCRR_INITIAL:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_CLOSE_SESSION_REQ:
        case TDSRRC_SIG_CONN_RELEASE_REQ:
          tdsrrcscrr_process_scrr_initial_substate(cmd_ptr);
          break;
        /*lint -e793*/
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"SCRR:Invalid event in Substate: %d",tdsrrcscrr_substate);
          break;
        /*lint +e793*/
      }
      break;

    case TDSRRCSCRR_WAIT_FOR_CELL_UPDATE_COMP_CNF: 
      tdsrrcscrr_process_scrr_wait_for_cell_update_comp_cnf_substate(cmd_ptr);
      break;

    default:           
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"SCRR: Invalid substate: %d in PCH",tdsrrcscrr_substate);
      break;
  }  
}/* tdsrrcscrr_process_cell_ura_pch_state */
/*===========================================================================

FUNCTION tdsrrcscrr_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the Signalling Connection Release
  Request procedure.
  The following events are handled by this procedure:

    TDSRRC_CLOSE_SESSION_REQ Primitive (received from MM)
    TDSRRC_CELL_UPDATE_COMPLETE_CNF primitive (received from CELL UPDATE Procedure)

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcscrr_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
)
{
 
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();
  if(TDSRRC_STATE_CHANGE_IND == cmd_ptr->cmd_hdr.cmd_id)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"State-Change-Ind scrr-state %d in rrc-state %d",tdsrrcscrr_substate,rrc_state);
    //Any pending message memory needs to be de-allocated here 
    tdsrrcscrr_delete_close_session_req(RRC_CS_DOMAIN_CN_ID);
    tdsrrcscrr_delete_close_session_req(RRC_PS_DOMAIN_CN_ID);
    tdsrrcscrr_substate = TDSRRCSCRR_INITIAL;  
  }
  else if(TDSRRC_SOFT_ABORT_GUARD_TIMER_EXPIRED_IND == cmd_ptr->cmd_hdr.cmd_id)
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"TDSRRC_SOFT_ABORT_GUARD_TIMER_EXPIRED_IND: scrr-state %d, idt-state %d,  udt-state %d",
                  tdsrrcscrr_substate, tdsrrcidt_substate, tdsrrcudt_substate);
    
    /*blindly close CS/PS domains as there is a check inside the called function*/
    tdsrrcscr_local_release_of_signalling_connection(RRC_CS_DOMAIN_CN_ID, RRC_SESSION_CLOSE_RL_FAILURE);
    tdsrrcscr_local_release_of_signalling_connection(RRC_PS_DOMAIN_CN_ID, RRC_SESSION_CLOSE_RL_FAILURE);

    /*call RCE to clean up and send abort cnf*/
    /*cnf to RCE is called by tdsrrcscrr_is_close_session_ind_needed inside above functions.*/

    /*force reset the substate*/
    tdsrrcscrr_substate = TDSRRCSCRR_INITIAL; 


  }

  else
  {
#if defined(FEATURE_3GPP_FAST_DORMANCY)
      if((TDSRRC_CLOSE_SESSION_REQ == cmd_ptr->cmd_hdr.cmd_id) ||
         (TDSRRC_SIG_CONN_RELEASE_REQ ==  cmd_ptr->cmd_hdr.cmd_id))
      {
        if(TRUE == tdsrrcdt_update_scrr_to_handle_dormancy(cmd_ptr))
          return;
      }
#endif 
 
    switch (rrc_state)
    {
      case TDSRRC_STATE_CELL_FACH:    
        tdsrrcscrr_process_cell_fach_state(cmd_ptr); 
        break;

      case TDSRRC_STATE_CELL_DCH:  
        tdsrrcscrr_process_cell_dch_state(cmd_ptr); 
        break;

      case TDSRRC_STATE_CELL_PCH:
      case TDSRRC_STATE_URA_PCH:
        tdsrrcscrr_process_cell_ura_pch_state(cmd_ptr);
        break;

      default:  
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"SCRR:Event in an invalid State: %d",rrc_state);
        break;
     }  
  }
} /* tdsrrcscrr_procedure_event_handler */

/*===========================================================================

FUNCTION tdsrrcddt_process_downlink_direct_transfer_msg  

DESCRIPTION
  This function checks whether TDSRRC_DOWNLINK_SDU_IND contains Downlink
  Direct Transfer message.If it contains Downlink Direct Transfer message,
  it will processes the same and send RRC_DATA_IND to the MM.In addition
  it also processes the TDSRRC_SMC_DDT_L2_ACK_IND command and forwards all
  the stored NAS messages to NAS.

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
static void tdsrrcddt_process_downlink_direct_transfer_msg
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr=NULL;    /* Pointer to Downlink DCCH */


  mm_cmd_type *mm_cmd_ptr;                   /* Pointer to the RRC Command */


  rlc_lc_id_type lc_id;                      /* Local variable to store 
                                                RLC LC Id */
  unsigned char *payload;                    /* Pointer to the MM payload */
 
  uint8 message_choice;                      /* Local varibale to store 
                                                message choice */
  uint8 count;                               /* Local counter for index */

  rrc_cn_domain_identity_e_type cn_id;

  /* Set the status to ASN1 successfully decoded */
  tdsrrc_protocol_error_reject = FALSE;


  /* Assert the received message is of type tdsrrc_DL_CCCH_Message */
  if((TDSRRC_DOWNLINK_SDU_IND == cmd_ptr->cmd_hdr.cmd_id) && 
     (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == tdsrrc_DL_DCCH_Message_PDU) )
  {
     message_choice = 
       tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                              cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);


    if(cmd_ptr->cmd.downlink_sdu_ind.decoded_msg != NULL)
    {
      /* Get the OTA message pointer */
      dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *) 
                     cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;
    }
    else
    {
      tdsrrc_protocol_error_reject = TRUE;
    }


    /* Check the received message is Downlink Direct transfer message */
    if( message_choice  ==
        T_tdsrrc_DL_DCCH_MessageType_downlinkDirectTransfer)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Downlink Direct Transfer Msg is rcvd");


      if(tdsrrc_protocol_error_reject == TRUE) /* DCCH ASN1 decoding failed */
      {
#if(!defined(T_WINNT))
        /*log the protocol error*/
        tdsrrc_log_protocol_error( (uint8) tdsrrc_get_state(),
                                (uint8)TDSRRC_PROCEDURE_DDT,
                                TDSRRCLOG_PROTOCOL_ERROR,
                                TDSRRCLOG_ASN1_ENC_ERR
                              );
#endif


        lc_id = tdsrrclcm_check_ul_rlc_lc_id(    
                UE_LOGCHAN_DCCH,          /* Uplink Logical Channel Type. */
                DCCH_AM_RADIO_BEARER_ID,  /* Radio Bearer Id assoicated with
                                            the above Downlink Logical 
                                            Channel type. */
                UE_MODE_ACKNOWLEDGED_DATA);
                                          /* Mode of transmission for the RLC 
                                             service entity. */
        if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND == lc_id)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Can not find RLC ID for UL RB 2");
        }         
        else /* Valid RLC ID found */
        {
          if(SUCCESS != tdsrrcsend_send_rrc_status_message(
             T_tdsrrc_ProtocolErrorMoreInformation_type1_asn1_ViolationOrEncodingError,
                                           // The protocol error cause to be placed in
                                           // the message
             0,                            // Set the transaction id to 0 since
                                           // ASN1 decoding is failed 
             tdsrrc_ReceivedMessageType_downlinkDirectTransfer,   // Received message identification
             TDSRRC_PROCEDURE_DDT,            // RRC procedure
             lc_id,                        /* Logical Channel Id */
             TDSRRCSEND_L2ACK_NOT_REQUIRED  ) ) 
                                           /* Indicates whether L2 Ack 
                                              required or not */
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unable to transmit RRC STATUS message");
          } 
        } /*  Valid RLC ID found */
      }
      else /* ASN1 decoding is successful */
      {
        /*lint -save -e613 */
        if (TDSRRC_CHECK_MSG_TYPE(dcch_msg_ptr->message.u.downlinkDirectTransfer,
                 T_tdsrrc_DownlinkDirectTransfer_later_than_r3))
        {
          /* Critical extensions are not supported */
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Critial extensions are not supported");


          lc_id = tdsrrclcm_check_ul_rlc_lc_id(    
                  UE_LOGCHAN_DCCH,          /* Uplink Logical Channel Type. */
                  DCCH_AM_RADIO_BEARER_ID,  /* Radio Bearer Id assoicated with
                                              the above Downlink Logical 
                                              Channel type. */
                  UE_MODE_ACKNOWLEDGED_DATA);
                                            /* Mode of transmission for the RLC 
                                               service entity. */
          if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND == lc_id)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Can not find RLC ID for UL RB 2");
          }         
          else /* Valid RLC ID found */
          {
            if(SUCCESS != tdsrrcsend_send_rrc_status_message(
               T_tdsrrc_ProtocolErrorMoreInformation_type1_messageExtensionNotComprehended,
                                             // The protocol error cause to be placed in
                                             // the message
               0,                            // Set the transaction id to 0 since
                                             // critical extensions selected
               tdsrrc_ReceivedMessageType_downlinkDirectTransfer,   // Received message identification
               TDSRRC_PROCEDURE_DDT,            // RRC procedure
               lc_id,                        /* Logical Channel Id */
               TDSRRCSEND_L2ACK_NOT_REQUIRED  ) ) 
                                             /* Indicates whether L2 Ack 
                                                required or not */
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unable to transmit RRC STATUS message");
            } 
          } /*  Valid RLC ID found */
          return;
        }
        /* Check for open signalling connection match */
        if( (( tdsrrc_CN_DomainIdentity_cs_domain ==
               dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.
               downlinkDirectTransfer_r3.cn_DomainIdentity) &&
               (tdsrrcscrr_is_cn_domain_open(RRC_CS_DOMAIN_CN_ID))) || 
            (( tdsrrc_CN_DomainIdentity_ps_domain == 
               dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.
               downlinkDirectTransfer_r3.cn_DomainIdentity) &&
               (tdsrrcscrr_is_cn_domain_open(RRC_PS_DOMAIN_CN_ID))) )
        {        
          /* Check for valid payload length */
          if((dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.
              downlinkDirectTransfer_r3.nas_Message.numocts == 0) ||
             (dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.
              downlinkDirectTransfer_r3.nas_Message.numocts >
              TDSRRCDT_MAX_PAYLOAD) )
          {
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"Invalid message length for DDT: %d", 
                      dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.
                      downlinkDirectTransfer_r3.nas_Message.numocts,0,0);
          }
          else /* Valid Message Length */
          {
             if(tdsrrc_CN_DomainIdentity_cs_domain ==
               dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.
               downlinkDirectTransfer_r3.cn_DomainIdentity)
            {
              cn_id = RRC_CS_DOMAIN_CN_ID;
            }
            else
            {
              cn_id = RRC_PS_DOMAIN_CN_ID;
            }

            /* Check whether Security mode procedure is active */
            if ((tdsrrcsmc_get_smc_procedure_complete_status(cn_id) == FALSE)
#ifdef  FEATURE_TDSCDMA_SRNS_OLD_FRESH_BUFFER_DDT
                &&(tdsrrcsmc_get_smc_srns_pending_status_for_dt() == FALSE) /*will be set to FALSE except during RLC re-est*/
#endif
                )
            {
              /* Security mode procedure is not active */
              payload = 
             (unsigned char *) nas_mem_alloc(
                         dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.
                        downlinkDirectTransfer_r3.nas_Message.numocts);
              if (payload != NULL)
              {
                /* Memory allocation is sucessful. Copy the payload */
                memscpy(payload,
                       dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.
                       downlinkDirectTransfer_r3.nas_Message.numocts,
                       dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.
                       downlinkDirectTransfer_r3.nas_Message.data,
                       dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.
                       downlinkDirectTransfer_r3.nas_Message.numocts);
  
                /* Allocate memory for RRC_DATA_IND command */
                if( (mm_cmd_ptr =  mm_tdsrrc_get_cmd_buf(RRC_DATA_IND)) != NULL)
                {
                  /* Fill in parameters for RRC_DATA_IND command */

#ifdef FEATURE_DUAL_SIM
                  mm_cmd_ptr->cmd.rrc_data_ind.as_id = tdsrrc_get_as_id();
#endif

                  if(dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.
                    downlinkDirectTransfer_r3.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain)
                  {   
                    mm_cmd_ptr->cmd.rrc_data_ind.cn_domain_id = RRC_CS_DOMAIN_CN_ID;
                  }
                  else
                  {
                    mm_cmd_ptr->cmd.rrc_data_ind.cn_domain_id = RRC_PS_DOMAIN_CN_ID;

                  }
                    
                  /*Perform check to see if IDT waiting for L2 Ack*/
                  if(tdsrrcidt_is_idt_waiting_for_l2ack_per_domain(mm_cmd_ptr->cmd.rrc_data_ind.cn_domain_id))
                  {
                      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Treating DDT as Implicit L2 Ack for IDT in %d Domain",mm_cmd_ptr->cmd.rrc_data_ind.cn_domain_id);
                      tdsrrcidt_send_open_session_cnf(RRC_OPEN_SESS_SUCCESS,  mm_cmd_ptr->cmd.rrc_data_ind.cn_domain_id);

                      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Security NV settings: Integrity %d, Ciphering_status %d, Fake %d",
                                                   tdsnv_integrity_enabled,
                                                   tdsnv_ciphering_enabled_status,
                                                   tdsnv_fake_security_enabled);
            
                      /* ZZ: sends RRC_SYNC_IND so that NAS won't trash the following DLDT msg */
                      if(tdsnv_fake_security_enabled)
                      {
                         tdsrrcidt_send_sync_ind(RRC_INTEGRITY_ESTABLISHED, mm_cmd_ptr->cmd.rrc_data_ind.cn_domain_id);
                      }
                  }
                  /*End of check*/  
  
                  mm_cmd_ptr->cmd.rrc_data_ind.nas_msg.length =
                    dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.
                    downlinkDirectTransfer_r3.nas_Message.numocts;               
  
                  mm_cmd_ptr->cmd.rrc_data_ind.nas_msg.value = payload;
                 
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sent RRC_DATA_IND to the MM");
  
                  /* Sends the command to MM */
                  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
                  {
                    tdscfa_mm_put_cmd(mm_cmd_ptr);
                  }
                  mm_put_cmd(mm_cmd_ptr);
                }
                else
                {
                  ERR_FATAL("Failed allocated memory for MM Cmd: %d",RRC_DATA_IND,0,0);
                }
              } /* Memory allocation for paylod failed */
              else
              {
                // Do nothing. Just drop the message.
              } 
            }
            else /* Security mode procedure is active */
            {
              /* Save the received message in the internal buffer until security mode 
                 procedure is completed */
              
              for (count = 0; count < (MAX_NAS_MESSAGES_FOR_CS_DOMAIN + 
                                       TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN); count++)
              {
                if(tdsrrcddt_saved_data[count].data_ind_status == FALSE)
                {
                  break;
                }
              }
              if (count < (MAX_NAS_MESSAGES_FOR_CS_DOMAIN + 
                           TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN) )
              {
                if( tdsrrc_CN_DomainIdentity_cs_domain ==
                    dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.
                    downlinkDirectTransfer_r3.cn_DomainIdentity)
                {
                  tdsrrcddt_saved_data[count].data_ind.cn_domain_id = RRC_CS_DOMAIN_CN_ID;
                }
                else if(tdsrrc_CN_DomainIdentity_ps_domain ==
                  dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.
                  downlinkDirectTransfer_r3.cn_DomainIdentity)
                {
                  tdsrrcddt_saved_data[count].data_ind.cn_domain_id = RRC_PS_DOMAIN_CN_ID;
                }
                else
                {
                  ERR_FATAL("Invalid CN domain Id",0,0,0);
                }
                /* Update the length of NAS message */


                tdsrrcddt_saved_data[count].data_ind.nas_msg.length = 
                  dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.
                  downlinkDirectTransfer_r3.nas_Message.numocts;

                /* Allocate memory for NAS messages */
                tdsrrcddt_saved_data[count].data_ind.nas_msg.value = 
             (unsigned char *) nas_mem_alloc(
                                    dcch_msg_ptr->message.u.
                  downlinkDirectTransfer.u.r3.downlinkDirectTransfer_r3.nas_Message.numocts);
                if(tdsrrcddt_saved_data[count].data_ind.nas_msg.value == NULL)
                {
                  ERR_FATAL("Mem malloc failed",0,0,0);
                }
                /* Memory allocation is sucessful. Copy the payload */
                memscpy(tdsrrcddt_saved_data[count].data_ind.nas_msg.value,
                  dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.
                  downlinkDirectTransfer_r3.nas_Message.numocts,
                  dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.
                  downlinkDirectTransfer_r3.nas_Message.data,
                  dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.
                  downlinkDirectTransfer_r3.nas_Message.numocts);


                /* Set the status Flag to TRUE indicating that message is saved*/
                tdsrrcddt_saved_data[count].data_ind_status = TRUE;
              }
            }/* Security procedure is active */
          }
        }
        else
        {
          /* Signalling Connection is not open. Reject the message */
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"No Signalling Connection is open: %d ", 
                    dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.
                    downlinkDirectTransfer_r3.cn_DomainIdentity,0,0);


          lc_id = tdsrrclcm_check_ul_rlc_lc_id(    
                  UE_LOGCHAN_DCCH,          /* Uplink Logical Channel Type. */
                  DCCH_AM_RADIO_BEARER_ID,  /* Radio Bearer Id assoicated with
                                              the above Downlink Logical 
                                              Channel type. */
                  UE_MODE_ACKNOWLEDGED_DATA);
                                            /* Mode of transmission for the RLC 
                                               service entity. */
          if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND == lc_id)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Can not find RLC ID for UL RB 2");
          }         
          else /* Valid RLC ID found */
          {
            if(SUCCESS != tdsrrcsend_send_rrc_status_message(
               T_tdsrrc_ProtocolErrorMoreInformation_type1_messageNotCompatibleWithReceiverState,
                                           // The protocol error cause to be 
                                           // placed in the message
               dcch_msg_ptr->message.
               u.downlinkDirectTransfer.u.r3.
               downlinkDirectTransfer_r3.
               rrc_TransactionIdentifier ,   // Set the transaction 
               tdsrrc_ReceivedMessageType_downlinkDirectTransfer,   // Received message identification
               TDSRRC_PROCEDURE_DDT,            // RRC procedure
               lc_id,                        /* Logical Channel Id */
               TDSRRCSEND_L2ACK_NOT_REQUIRED)   /* Indicates whether L2 Ack */
              )                              /* required or not */
                                               
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to xmit RRC STATUS msg");
            }
            else
            { 
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC STATUS is sent");
            }
          } /*  Valid RLC ID found */
        } 
        /*lint -restore */
      } 
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Msg: %d is fwded to DDT Proc",message_choice);
    }
  }
  else if((TDSRRC_SMC_DDT_L2_ACK_IND == cmd_ptr->cmd_hdr.cmd_id)
#ifdef  FEATURE_TDSCDMA_SRNS_OLD_FRESH_BUFFER_DDT
          ||(TDSRRC_SMC_SRNS_RLC_RE_EST_CMP == cmd_ptr->cmd_hdr.cmd_id)
#endif           
          )
  {

    TDSRRC_MSG1(MSG_LEGACY_HIGH, "POP buffered DDT msg to NAS,cmd id = 0x%X",cmd_ptr->cmd_hdr.cmd_id);

    for (count = 0; count < (MAX_NAS_MESSAGES_FOR_CS_DOMAIN + 
                                   TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN); count++)
    {
      /* Check if any messages are queued */
      if(tdsrrcddt_saved_data[count].data_ind_status == TRUE)
      {
        /* Allocate memory for RRC_DATA_IND command */
        if( (mm_cmd_ptr =  mm_tdsrrc_get_cmd_buf(RRC_DATA_IND)) != NULL)
        {
          /* Fill in parameters for RRC_DATA_IND command */

          mm_cmd_ptr->cmd.rrc_data_ind.cn_domain_id = 
            tdsrrcddt_saved_data[count].data_ind.cn_domain_id;


          mm_cmd_ptr->cmd.rrc_data_ind.nas_msg.length =
            tdsrrcddt_saved_data[count].data_ind.nas_msg.length;               


          mm_cmd_ptr->cmd.rrc_data_ind.nas_msg.value = 
            tdsrrcddt_saved_data[count].data_ind.nas_msg.value;

#ifdef FEATURE_DUAL_SIM
          mm_cmd_ptr->cmd.rrc_data_ind.as_id = tdsrrc_get_as_id();
#endif

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sent RRC_DATA_IND to the MM");


          /* Sends the command to MM */
          if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
          {
            tdscfa_mm_put_cmd(mm_cmd_ptr);
          }
          mm_put_cmd(mm_cmd_ptr);

          tdsrrcddt_saved_data[count].data_ind_status = FALSE;
        }
        else
        {
          ERR_FATAL("Failed allocated memory for MM Cmd: %d",RRC_DATA_IND,0,0);
        }
      } /* Check if any messages are queued */ 
    }

  }
  else
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d or SDU type: %d to DDT",cmd_ptr->cmd_hdr.cmd_id,cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type);
  }


} /* tdsrrcddt_process_downlink_direct_transfer_msg */
/*===========================================================================

FUNCTION tdsrrcddt_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the downlink direct transfer
  procedure.
  The following events are handled by this procedure:

    TDSRRC_DOWNLINK_SDU_IND Primitive (with DOWNLINK DIRECT TRANSFER MESSAGE)


DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcddt_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
)
{
 
  uint32 count;                               //for counter purposes

  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  //process state_change_ind right here just to be sure that we don't
  //miss going to idle at any cost.. clear saved_data_req allocs if present
  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_STATE_CHANGE_IND)
  {
    //loop thru list of msgs in the saved_data database and delete them
    //if saved data flag is true, also reset the indication status
    for (count = 0; count < (MAX_NAS_MESSAGES_FOR_CS_DOMAIN + 
                             TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN); count++)
    {
      if(tdsrrcddt_saved_data[count].data_ind_status == TRUE)
      {
        modem_mem_free(tdsrrcddt_saved_data[count].data_ind.nas_msg.value, 
        MODEM_MEM_CLIENT_NAS_CRIT);
        tdsrrcddt_saved_data[count].data_ind_status = FALSE;
      }
    }
  }
  else
  {
    switch (rrc_state)
    {
      case TDSRRC_STATE_DISCONNECTED: 
      case TDSRRC_STATE_CONNECTING:
        /* In these states there is no RRC Connection and Signalling Connection. Hence
           trash the message */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"DDT Msg received in invalid state: %d",rrc_state); 
        break;

      case TDSRRC_STATE_CELL_FACH:    
      case TDSRRC_STATE_CELL_DCH:  
        /* These two states are valid to receive Downlink Direct Transfer message */
        tdsrrcddt_process_downlink_direct_transfer_msg(cmd_ptr); 
        break;

      case TDSRRC_STATE_CELL_PCH:
      case TDSRRC_STATE_URA_PCH:
        /* The logical DCCH is released in these states.Due to race conditions, RRC
           may receive a valid DOWNLINK DIRECT TRANSFER message. Hence process the same
           and send the Payload to NAS. */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"DDT msg is recieved in PCH state");
        tdsrrcddt_process_downlink_direct_transfer_msg(cmd_ptr);
        break;

      default: 
        /* This should never happen */
        ERR_FATAL("Event fwded to DDT in invalid State: %d",rrc_state,0,0);
        break;
    }  
  }
} /* tdsrrcddt_procedure_event_handler */
/*===========================================================================

FUNCTION tdsrrcscr_send_close_session_ind  

DESCRIPTION
  This function builds the RRC_CLOSE_SESSION_IND command with parameters and 
  posts the same to the MM task.
DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcdt_status_e_type: It returns TDSRRCDT_SUCCESS if it is successfully sends 
  the RRC_CLOSE_SESSION_IND command. Otherwise it returns TDSRRCDT_FAILURE_MAJOR.

===========================================================================*/

tdsrrcdt_status_e_type tdsrrcscr_send_close_session_ind
(
  rrc_cn_domain_identity_e_type       cn_domain_id,     /* CN Domain Id */
  rrc_session_close_cause_e_type      cause
)
{
  mm_cmd_type *cmd_ptr;                      /* Pointer to the MM Command */

  tdsrrcdt_status_e_type status = TDSRRCDT_FAILURE_MAJOR; 
                                             /* The status is initialized to
                                                Failure */

#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"CSFB_HACK: tdsrrc_csfb_extended_status = %d, close session for domain %d, PS domain status %d",
            tdsrrc_csfb_extended_status,cn_domain_id,tdsrrcdt_int_data.ps_domain_open);
  if((tdsrrc_csfb_extended_status == CSFB_STARTED) &&
      (cn_domain_id == RRC_CS_DOMAIN_CN_ID)
#ifdef FEATURE_WRLF_SYSTEM_SEL
       /* Send close session ind to NAS if CS domain is released because of T314 timer expiry. */
       && !(TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
#endif
       )
  {
    tdsrrcrce_post_internal_csfb_abort_cmd();
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"CSFB_HACK: tdsrrc_csfb_extended_status = %d",tdsrrc_csfb_extended_status);
    return TDSRRCDT_SUCCESS;
  }
#endif

  /* Allocate memory for RRC_OPEN_SESSION_CNF command */
  if( (cmd_ptr =  mm_tdsrrc_get_cmd_buf(RRC_CLOSE_SESSION_IND)) != NULL)
  {
    /* Fill in parameters fro RRC_OPEN_SESSION_CNF command */
    cmd_ptr->cmd.rrc_close_session_ind.cn_domain_id = cn_domain_id ;


    tdsrrcsmc_update_ciphering_config_info_for_signalling_release(cn_domain_id);
    tdsrrcsmc_update_sync_ind_info_for_signalling_release(cn_domain_id);

     // No causes defined in the interface file
    cmd_ptr->cmd.rrc_close_session_ind.cause = cause;

#ifdef FEATURE_DUAL_SIM
    cmd_ptr->cmd.rrc_close_session_ind.as_id = tdsrrc_get_as_id();
#endif

    /* Sends the command to MM */
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(cmd_ptr);
    }
    mm_put_cmd(cmd_ptr);
    status = TDSRRCDT_SUCCESS;            /* sets the status to success */
  }    
  else   /* RRC Task failed to allocate memory */
  {
    ERR_FATAL("Unable to allocate memory for MM Cmd: %d",RRC_CLOSE_SESSION_IND,0,0);
  }     
  return (status);

} /* tdsrrcscr_send_close_session_ind */

/*===========================================================================

FUNCTION tdsrrcscr_reset_signalling_connection_status

DESCRIPTION
  This function sets both CS and PS signalling Connections to release status.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcscr_reset_signalling_connection_status
(
  void                    
)
{

  /* Initialize the flag to show that a signalling session is not open to
   * the CS domain
   */
  tdsrrcdt_int_data.cs_domain_open = FALSE;

  /* Initialize the flag to show that a signalling session is not open to
   * the PS domain
   */
  tdsrrcdt_int_data.ps_domain_open = FALSE;

} /* tdsrrcscr_reset_signalling_connection_status */

/*===========================================================================

FUNCTION tdsrrcscr_check_active_rabs_in_sig_conn_rel_domain

DESCRIPTION
  This function checks if active rabs are present in the CN Domain for which
  we got Signalling_Connection_Release


DEPENDENCIES
  None.

RETURN VALUE
  rrcscr_sig_conn_statue_e_type: it returns TDSRRCSCR_RAB_PRESENT_IN_SIG_DOMAIN
  if successful, else returns TDSRRCSCR_NO_RAB_PRESENT_IN_SIG_DOMAIN

===========================================================================*/
static tdsrrcscr_sig_conn_rel_status_e_type tdsrrcscr_check_active_rabs_in_sig_conn_rel_domain
(
  rrc_cn_domain_identity_e_type cn_domain_id
)
{
  uint32 count,i;                  /* count for counter purposes          */

  /*loop through established_rabs struct and check to see if any active rabs
    are present in the domain for which we are trying to do a SCR*/
  for ( count=0; count<MAX_RAB_TO_SETUP; count++ )
  {
    if (tdsrrc_est_rabs.rabs[count].rab_id != TDSRRC_INVALID_RAB_ID)
    { 
      for (i=0; i< tdsrrc_est_rabs.rabs[count].num_rbs_for_rab; i++)
      {
        if ((tdsrrc_est_rabs.rabs[count].rb_for_rab[i].rb_status ==
             TDSRRC_RB_STARTED) && (tdsrrc_est_rabs.rabs[count].cn_domain == 
          cn_domain_id))
        {
          //we can return right away
          return TDSRRCSCR_RAB_PRESENT_IN_SIG_DOMAIN;
        }
        else
        {
          //this rab does not have any RB's for the domain which is being released.
          //therefore continue to the next RAB as all RB's in a RAB will belong to 
          //same CN domain
          break; 
        }
      }
    }
  }

  return TDSRRCSCR_NO_RAB_PRESENT_IN_SIG_DOMAIN;
}/*tdsrrcscr_check_active_rabs_in_sig_conn_rel_domain*/
/*===========================================================================

FUNCTION tdsrrcscr_process_signaling_connection_release_msg  

DESCRIPTION
  This function checks whether TDSRRC_DOWNLINK_SDU_IND contains Signalling
  connection release message.If it contains Signalling Connection Release
  message, it will processes the same and send RRC_CLOSE_SESSION_IND to the MM.

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/

static void tdsrrcscr_process_signaling_connection_release_msg
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr=NULL;    /* Pointer to Downlink DCCH */

  rlc_lc_id_type lc_id;                      /* Local variable to store 
                                                RLC Logical Channel Id */
  rrc_cn_domain_identity_e_type cn_domain_id; /* Local variable for CN Domain */

  uint8 message_choice;                       /* Local variable for message
                                                 choice */

  /* Set the protocol error to FALSE */
  tdsrrc_protocol_error_reject = FALSE;

  /* Assert the receieved message is of type tdsrrc_DL_CCCH_Message */

  if((TDSRRC_DOWNLINK_SDU_IND == cmd_ptr->cmd_hdr.cmd_id) && 
     (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == tdsrrc_DL_DCCH_Message_PDU) )
  {
    message_choice = 
      tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                            cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);

    if (cmd_ptr->cmd.downlink_sdu_ind.decoded_msg != NULL)
    {
      /* Get the OTA message pointer */
      dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *) 
                     cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;
    }
    else
    {
      /* ASN1 decoding failed */
      tdsrrc_protocol_error_reject = TRUE;
    }
 
    /* Check the received message is Signalling Connection Release message */
    if( message_choice == 
        T_tdsrrc_DL_DCCH_MessageType_signallingConnectionRelease)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Signalling Connection Rel is received");

      if(tdsrrc_protocol_error_reject == TRUE) /* DCCH ASN1 decoding failed */
      {

#if(!defined(T_WINNT))
        /*log the protocol error*/
        tdsrrc_log_protocol_error( (uint8) tdsrrc_get_state(),
                                (uint8)TDSRRC_PROCEDURE_SCRR,
                                TDSRRCLOG_PROTOCOL_ERROR,
                                TDSRRCLOG_ASN1_ENC_ERR
                              );
#endif

        lc_id = tdsrrclcm_check_ul_rlc_lc_id(    
                UE_LOGCHAN_DCCH,          /* Uplink Logical Channel Type. */
                DCCH_AM_RADIO_BEARER_ID,  /* Radio Bearer Id assoicated with
                                            the above Downlink Logical 
                                            Channel type. */
                UE_MODE_ACKNOWLEDGED_DATA);
                                          /* Mode of transmission for the RLC 
                                             service entity. */
        if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND == lc_id)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Can't find RLC ID for UL RB 2");
        } 
        else /* Valid RLC ID found */
        {
          if(SUCCESS != tdsrrcsend_send_rrc_status_message(
             T_tdsrrc_ProtocolErrorMoreInformation_type1_asn1_ViolationOrEncodingError,
                                           // The protocol error cause to be placed in
                                           // the message
             0,                            // Set the transaction id to 0 since 
                                           // ASN1 decoding failed
             tdsrrc_ReceivedMessageType_signallingConnectionRelease,
                                           // Received Msg Identifcation
             TDSRRC_PROCEDURE_SCR,            // RRC procedure
             lc_id,                        /* Logical Channel Id */
             TDSRRCSEND_L2ACK_NOT_REQUIRED  ) ) 
                                           /* Indicates whether L2 Ack 
                                              required or not */
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unable to transmit RRC STATUS message");
          } 
          
        } /* Valid RLC ID found */
        
      } 
      else /* ASN1 decoding is successful */
      {
        /*lint -save -e613 */
        if(TDSRRC_CHECK_MSG_TYPE(dcch_msg_ptr->message.u.signallingConnectionRelease,
                      T_tdsrrc_DL_DCCH_MessageType_signallingConnectionRelease))
        {
          /* Critical extensions are not supported */
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Critical extensions are not supported");
          return;
        }
        
        /* Update CN Domain ID */
        if(dcch_msg_ptr->message.u.signallingConnectionRelease.u.r3.
              signallingConnectionRelease_r3.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain)
        {
          cn_domain_id = RRC_CS_DOMAIN_CN_ID;
        }
        else
        {
          cn_domain_id  = RRC_PS_DOMAIN_CN_ID;
        }

        if(tdsrrcscr_check_active_rabs_in_sig_conn_rel_domain(cn_domain_id) == 
           TDSRRCSCR_RAB_PRESENT_IN_SIG_DOMAIN)
        {
#if(!defined(T_WINNT))
            /*log the protocol error*/
          tdsrrc_log_protocol_error( (uint8) tdsrrc_get_state(),
                                  (uint8)TDSRRC_PROCEDURE_SCRR,
                                  TDSRRCLOG_PROTOCOL_ERROR,
                                  TDSRRCLOG_ASN1_ENC_ERR
                                );
#endif
    
          lc_id = tdsrrclcm_check_ul_rlc_lc_id(    
                  UE_LOGCHAN_DCCH,          /* Uplink Logical Channel Type. */
                  DCCH_AM_RADIO_BEARER_ID,  /* Radio Bearer Id assoicated with
                                              the above Downlink Logical 
                                              Channel type. */
                  UE_MODE_ACKNOWLEDGED_DATA);
                                              /* Mode of transmission for the RLC 
                                                 service entity. */
          if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND == lc_id)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Can't find RLC ID for UL RB 2");
          } 
          else /* Valid RLC ID found */
          {
            if(SUCCESS != tdsrrcsend_send_rrc_status_message(
               T_tdsrrc_ProtocolErrorMoreInformation_type1_messageNotCompatibleWithReceiverState,
                                             // The protocol error cause to be placed in
                                             // the message
               dcch_msg_ptr->message.u.signallingConnectionRelease.u.r3.
               signallingConnectionRelease_r3.rrc_TransactionIdentifier,                            
                                            // Set the transaction id to 0 since 
               tdsrrc_ReceivedMessageType_signallingConnectionRelease,
                                             // Received Msg Identifcation
               TDSRRC_PROCEDURE_SCR,            // RRC procedure
               lc_id,                        /* Logical Channel Id */
               TDSRRCSEND_L2ACK_NOT_REQUIRED  ) ) 
                                             /* Indicates whether L2 Ack 
                                                required or not */
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unable to transmit RRC STATUS message");
            } 
              
          } /* Valid RLC ID found */
        }
        else//only if active rab is not found in CN Domain we r trying to release
        {
   
          /* Check for open signalling connection match */
          if((( RRC_CS_DOMAIN_CN_ID == cn_domain_id) &&
               (tdsrrcdt_int_data.cs_domain_open)) ||
             (( RRC_PS_DOMAIN_CN_ID == cn_domain_id) && 
                (tdsrrcdt_int_data.ps_domain_open)) )
          {        
            if(RRC_CS_DOMAIN_CN_ID == cn_domain_id)
            {
              if(mm_per_subs_get_cs_domain_no_rsp_type(tdsrrc_get_as_id()) == CALL_STATE_MO_REJECT_CAUSE15)
              {           
                tdsrrc_cs_domain_error.mo_error_count++;
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"CS_domain_error: CALL_STATE_MO_REJECT_CAUSE15 after calling NAS API after receiving SCRI, mo_error_count:%d", tdsrrc_cs_domain_error.mo_error_count);                
              }
              else if(mm_per_subs_get_cs_domain_no_rsp_type(tdsrrc_get_as_id()) == CALL_STATE_MT_PAGE_NO_SETUP)
              {
                tdsrrc_cs_domain_error.mt_error_count++;
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"CS_domain_error: CALL_STATE_MT_PAGE_NO_SETUP after calling NAS API after receiving SCRI, mt_error_count:%d", tdsrrc_cs_domain_error.mt_error_count);                
              }
			  else
			  {
			    TDSRRC_MSG0(MSG_LEGACY_HIGH,"CS_domain_error: reset error count");                
			    tdsrrc_cs_domain_error.mo_error_count = 0;
                tdsrrc_cs_domain_error.mt_error_count = 0;
			  }
              if((tdsrrc_cs_domain_error.mo_error_count == TDSMAX_CS_DOMAIN_MO_ERROR_COUNT) ||
                  (tdsrrc_cs_domain_error.mt_error_count == TDSMAX_CS_DOMAIN_MT_ERROR_COUNT))
              {
                if (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)
                {
                  tdsrrcdt_send_cell_update_initiate_req(TDSRRC_RADIO_LINK_FAILURE);
                }
                tdsrrccsp_send_cphy_cell_bar_req(RRC_CSP_TDS_CPID_BARRED, (uint32)tds_cs_domain_bar_timer_length); 
                tdsrrc_cs_domain_error.mo_error_count = 0;
                tdsrrc_cs_domain_error.mt_error_count = 0;                
              }
            }

            /*On receiving SCR from NW when there is a pending close session req, RRC should send Close session cnf*/
            if((cn_domain_id == RRC_CS_DOMAIN_CN_ID) &&
               (tdsrrcscrr_saved_cs_close_session_req.mui_status == TDSRRCSCRR_MUI_CLOSE_SESSION_CNF))
            {
              tdsrrcscrr_send_close_session_cnf(RRC_CLOSE_SESS_SUCCESS, RRC_CS_DOMAIN_CN_ID);
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Interpreting SCR message for CS domain as RLC ACK for SCRI");
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"CS DOMAIN CLOSE SESSION CNF WITH SUCCESS - DELETE ALL SAVED UDTs");
              tdsrrcudt_delete_all_saved_data_req(RRC_CS_DOMAIN_CN_ID);
              tdsrrcdt_int_data.cs_domain_open = FALSE;
              if(tdsrrcudt_any_saved_data_req_for_cn_domain(RRC_PS_DOMAIN_CN_ID) == FALSE)
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"There is no saved UDT for PS domain,set udt substate to init");
                tdsrrcudt_substate = TDSRRCUDT_INITIAL;
              }
              
              if(tdsrrcscrr_saved_ps_close_session_req.mui_status != TDSRRCSCRR_MUI_STATUS_FALSE)
              {
                tdsrrcscrr_substate = TDSRRCSCRR_WAIT_FOR_L2ACK; 
              }
              else
              {
                tdsrrcscrr_substate = TDSRRCSCRR_INITIAL; 
              }
              tdsrrcscrr_delete_close_session_req(RRC_CS_DOMAIN_CN_ID);
            }
            else if(( RRC_PS_DOMAIN_CN_ID == cn_domain_id) &&  
                      (tdsrrcscrr_saved_ps_close_session_req.mui_status == TDSRRCSCRR_MUI_CLOSE_SESSION_CNF))     
            {
              tdsrrcscrr_send_close_session_cnf(RRC_CLOSE_SESS_SUCCESS, RRC_PS_DOMAIN_CN_ID);
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Interpreting SCR message for PS domain as RLC ACK for SCRI");
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"PS DOMAIN CLOSE SESSION CNF WITH SUCCESS - DELETE ALL SAVED UDTs");
              tdsrrcudt_delete_all_saved_data_req(RRC_PS_DOMAIN_CN_ID);
              tdsrrcdt_int_data.ps_domain_open = FALSE;
              if(tdsrrcudt_any_saved_data_req_for_cn_domain(RRC_CS_DOMAIN_CN_ID) == FALSE)
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"There is no saved UDT for CS domain,set udt substate to init");
                tdsrrcudt_substate = TDSRRCUDT_INITIAL;
              }
              if(tdsrrcscrr_saved_cs_close_session_req.mui_status != TDSRRCSCRR_MUI_STATUS_FALSE)
              {
                tdsrrcscrr_substate = TDSRRCSCRR_WAIT_FOR_L2ACK; 
              }
              else
              {
                tdsrrcscrr_substate = TDSRRCSCRR_INITIAL; 
              }
              tdsrrcscrr_delete_close_session_req(RRC_PS_DOMAIN_CN_ID);
            }
            else
            if(TDSRRCDT_SUCCESS == tdsrrcscr_send_close_session_ind(cn_domain_id, RRC_MAX_SESSION_CLOSE_CAUSE))
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sent RRC_CLOSE_SESSION_IND");
              switch(cn_domain_id)
              {
                case RRC_CS_DOMAIN_CN_ID:
                  tdsrrcidt_clear_idt_data_for_race_conditions(RRC_CS_DOMAIN_CN_ID);
                  /* Set the Signalling Connection status to closed */
                  tdsrrcdt_int_data.cs_domain_open = FALSE;
                  break;
     
                case RRC_PS_DOMAIN_CN_ID:
                  /* Set the Signalling Connection Status to closed */
                  tdsrrcidt_clear_idt_data_for_race_conditions(RRC_PS_DOMAIN_CN_ID);
                  tdsrrcdt_int_data.ps_domain_open = FALSE;
#if defined(FEATURE_3GPP_FAST_DORMANCY)
                  /* Abort Dormancy even if SCRI for CLOSE_SESSION_REQ is sended */
                  tdsrrcdormancy_check_and_abort_dormancy_req(RRC_FAST_DORMANCY_SUCCESS_APP_NO_RETRY);
#endif                    
                  break;
    
                default:
                  ERR_FATAL("This should never_happen",0,0,0);
                  break;
              }
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to post RRC_CLOSE_SESSION_IND");
            }
          }
          else
          {
            /* Signalling Connection is not open. Trash the message */
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"No Signalling Connection is open %d",cn_domain_id);
          } 
        }
        /*lint -restore */
      } 
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Msg: %d is forwarded to SCR Proc",message_choice);
    }
  }
  else
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d or SDU type: %d to SCR",cmd_ptr->cmd_hdr.cmd_id,cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type);
  }

} /* tdsrrcscr_process_signaling_connection_release_msg */
/*===========================================================================

FUNCTION tdsrrcscr_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the signalling Connection Release
  procedure.
  The following events are handled by this procedure:

    TDSRRC_DOWNLINK_SDU_IND Primitive (with SIGNALLING CONNECTION RELEASE MESSAGE)


DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcscr_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
)
{
 
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  switch (rrc_state)
  {
    case TDSRRC_STATE_DISCONNECTED: 
    case TDSRRC_STATE_CONNECTING:
      /* In these states there is no RRC Connection and Signalling Connection. Hence
         trash the message */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"SCR Msg is rcvd in invalid state: %d",rrc_state); 
      break;

    case TDSRRC_STATE_CELL_FACH:    
    case TDSRRC_STATE_CELL_DCH:  
      /* These two states are valid to receive Downlink Direct Transfer message */
      tdsrrcscr_process_signaling_connection_release_msg(cmd_ptr); 
      break;

    case TDSRRC_STATE_CELL_PCH:
    case TDSRRC_STATE_URA_PCH:
      /* The logical DCCH is released in these states.Due to race conditions, RRC
         may receive a valid SIGNALING CONNECTION RELEASE message. Hence process 
         the same and send the Payload to NAS. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SCR msg is recievd in PCH state");
      tdsrrcscr_process_signaling_connection_release_msg(cmd_ptr);
      break;

    default: 
      /* This should never happen */
      TDSRRC_MSG1(MSG_LEGACY_FATAL,"Event fwded to SCR in invalid State: %d",rrc_state);
      break;
   }  
} /* tdsrrcscr_procedure_event_handler */

/*===========================================================================

FUNCTION tdsrrcscr_release_signalling_connection

DESCRIPTION
  This function checks whether Signalling connection exist for the particular
  CN Domain and if it exits, then sends RRC_CLOSE_SESSION_IND to the MM. Also
  sets the local variable for signalling connection status to FALSE.


DEPENDENCIES
  None.

RETURN VALUE
  rrcscr_sig_conn_statue_e_type: it returns TDSRRCSCR_SIG_CONN_RELEASED if there
  exits a signalling connection for the CN domain specified in the formal 
  parameter and if RRCSCR procedure successfully sends RRC_CLOSE_SESSION_IND
  to the MM. Otherwise it retuns TDSRRCSCR_SIG_CONN_NOT_EXIST.

===========================================================================*/

tdsrrcscr_sig_conn_rel_status_e_type tdsrrcscr_release_signalling_connection
(
  rrc_cn_domain_identity_e_type cn_domain_id
)
{
  tdsrrcscr_sig_conn_rel_status_e_type status = TDSRRCSCR_SIG_CONN_NOT_EXIST;
                                   /* Local parameter to store the status */
  
  switch(cn_domain_id)
  {
    case RRC_CS_DOMAIN_CN_ID:
      /* Check whether Signalling Connection for CS Domain is open,ps_domain_open doesnt 
      * give correct status,So make sure IDT procedure is not in progress first case 
      * says that open session is received, eg waiting for CU
      * second condition says that we are waiting for l2ack
      */
      if((tdsrrcdt_int_data.cs_domain_open == TRUE) &&
         (tdsrrcidt_is_idt_active(RRC_CS_DOMAIN_CN_ID) == FALSE))
      {
        if(TDSRRCDT_SUCCESS == tdsrrcscr_indicate_close_session(cn_domain_id, RRC_MAX_SESSION_CLOSE_CAUSE))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sent RRC_CLOSE_SESSION_IND");

          /* Set the Signalling connection status to closed */
          tdsrrcdt_int_data.cs_domain_open = FALSE;

          status = TDSRRCSCR_SIG_CONN_RELEASED;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"CS DOMAIN CLOSE SESSION CNF WITH SUCCESS - DELETE ALL SAVED UDTs");
          tdsrrcudt_delete_all_saved_data_req(RRC_CS_DOMAIN_CN_ID);
          if(tdsrrcudt_any_saved_data_req_for_cn_domain(RRC_PS_DOMAIN_CN_ID) == FALSE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"There is no saved UDT for PS domain,set udt substate to init");
            tdsrrcudt_substate = TDSRRCUDT_INITIAL;
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to post RRC_CLOSE_SESSION_IND");
        }
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"No need to close Doamin =%d,domain_open=%d,IDT active =%d",
           cn_domain_id,tdsrrcdt_int_data.cs_domain_open,tdsrrcidt_is_idt_active(RRC_CS_DOMAIN_CN_ID));
      }

      break;

    case RRC_PS_DOMAIN_CN_ID:
      /* Check whether Signalling Connection for PS Domain is open,ps_domain_open doesnt 
      * give correct status,So make sure IDT procedure is not in progress first case 
      * says that open session is received, eg waiting for CU
      * second condition says that we are waiting for l2ack
      */
      if((tdsrrcdt_int_data.ps_domain_open == TRUE)&&
         (tdsrrcidt_is_idt_active(RRC_PS_DOMAIN_CN_ID) == FALSE))
      {
        if(TDSRRCDT_SUCCESS == tdsrrcscr_indicate_close_session(cn_domain_id, RRC_MAX_SESSION_CLOSE_CAUSE))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sent RRC_CLOSE_SESSION_IND");

          /* Set the Signalling connection status to closed */
          tdsrrcdt_int_data.ps_domain_open = FALSE;

          status = TDSRRCSCR_SIG_CONN_RELEASED;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"PS DOMAIN CLOSE SESSION CNF WITH SUCCESS - DELETE ALL SAVED UDTs");
          tdsrrcudt_delete_all_saved_data_req(RRC_PS_DOMAIN_CN_ID);
          if(tdsrrcudt_any_saved_data_req_for_cn_domain(RRC_CS_DOMAIN_CN_ID) == FALSE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"There is no saved UDT for CS domain,set udt substate to init");
            tdsrrcudt_substate = TDSRRCUDT_INITIAL;
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to post RRC_CLOSE_SESSION_IND");
        }
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"No need to close Doamin =%d,domain_open=%d,IDT active =%d",
           cn_domain_id,tdsrrcdt_int_data.ps_domain_open,tdsrrcidt_is_idt_active(RRC_PS_DOMAIN_CN_ID));
      }
      break;

    default:
      ERR_FATAL("This should never happen",0,0,0);
      break;
  }
  return (status);
} /* tdsrrcscr_release_signalling_connection */


/*===========================================================================

FUNCTION tdsrrcdt_get_nv_items

DESCRIPTION
  This function gets NV item NV_FAKE_SECURITY_ENABLED which 
  used to determine if Fake Security will be sent.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcdt_get_nv_items
(
  void
)
{
  /*read fake security ENABLE/DISABLE flag from NV*/
  if(tdsrrcnv_efs_read(TDS_RRC_NV_RRC_FAKE_SECURITY_ENABLED,
                       (byte *) &tdsnv_fake_security_enabled,
                       sizeof(tdsnv_fake_security_enabled)))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS_RRC_NV_RRC_FAKE_SECURITY_ENABLED is %d",tdsnv_fake_security_enabled);
  }
  else
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_RRC_FAKE_SECURITY_ENABLED,
	                               (byte *) &tdsnv_fake_security_enabled,
								   sizeof(tdsnv_fake_security_enabled),
								   TRUE))
    {
      tdsnv_fake_security_enabled=0;  /*set fake security enabled to 0 as default*/
	  TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDS_RRC_NV_RRC_FAKE_SECURITY_ENABLED item error. Setting to default: %d",tdsnv_fake_security_enabled);
    }

    TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDS_RRC_NV_RRC_FAKE_SECURITY_ENABLED not read,setting to default: %d",tdsnv_fake_security_enabled);
  }

  /*if integrity is enabled, then fake security should be disabled.. only one of the 2
    nv items should be turned on*/
  if(tdsnv_integrity_enabled == 1)
  {
    tdsnv_fake_security_enabled = 0;
	TDSRRC_MSG0(MSG_LEGACY_HIGH,"integrity is enabled,disable fake security");
  }
} 

/*===========================================================================

FUNCTION tdsrrcdt_is_idt_waitingfor_l2ack

DESCRIPTION
  This function is used by other procedures like CU to see if dt has queued some messages to RLC

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

boolean tdsrrcdt_is_idt_waitingfor_l2ack(void)
{

   boolean ret_val=FALSE;
   if(tdsrrcidt_substate == TDSRRCIDT_WAIT_FOR_L2ACK)
   {
       ret_val =  TRUE;
   }
   return (ret_val);
}

/*===========================================================================

FUNCTION tdsis_ue_oos_and_em_call_attempted

DESCRIPTION
  This function takes UE to idle if UE is OOS (in Connected Mode) and an emergency call 
  is attempted
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE: UE is OOS and EM call is attempted. 
  FALSE: otherwise

===========================================================================*/
boolean tdsis_ue_oos_and_em_call_attempted
(
  tdsrrc_cmd_type    *cmd_ptr, 
  tdsrrc_proc_e_type rrc_proc
)
{
  boolean status = FALSE;

  /* If UE is OOS in Connected Mode and an emergency call is attempted then
   * take UE to idle
   */
  if(TDSRRC_NOT_CAMPED_ON == tdsrrcccm_get_curr_camping_status(rrc_proc))
  {
    switch (rrc_proc) 
    {
      case TDSRRC_PROCEDURE_IDT:
        if(cmd_ptr->cmd.open_sess_req.est_cause == RRC_EST_EMERGENCY_CALL)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"idt: UE OOS in conn st %d when EM call attmpted. Tx to discon",tdsrrc_get_state());

          /* Send initiate cell sel command to CSP so that UE is taken to Idle */
          (void)tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_IDT, TDSRRC_TX_TO_DISCON_EM_CALL);

          /* Initialize state of the variables */
          tdsrrcidt_delete_open_session_req(RRC_CS_DOMAIN_CN_ID);  
          tdsrrcidt_delete_open_session_req(RRC_PS_DOMAIN_CN_ID);  
          tdsrrc_release_cause = RRC_REL_DEEP_FADE;
          status = TRUE;
        }
        break;

      case TDSRRC_PROCEDURE_UDT:
        if(cmd_ptr->cmd.data_req.is_emergency_call == TRUE)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"udt: UE OOS in conn st %d when EM call attmpted. Tx to discon",tdsrrc_get_state());

          /* Send initiate cell sel command to CSP so that UE is taken to Idle */
          (void)tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_UDT, TDSRRC_TX_TO_DISCON_EM_CALL);

          /* Initialize state of the variables */
          tdsrrcudt_delete_all_saved_data_req(RRC_CS_DOMAIN_CN_ID);
          tdsrrcudt_delete_all_saved_data_req(RRC_PS_DOMAIN_CN_ID);
          tdsrrcudt_substate = TDSRRCUDT_INITIAL;
          tdsrrc_release_cause = RRC_REL_DEEP_FADE;
          status = TRUE;
        }
        break;

      default: 
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: Incorrect proc_id %d passed to the func",rrc_proc);
        break;
    }/* end switch statement */
  } /* end if statement */

  return (status);
} /* tdsis_ue_oos_and_em_call_attempted */

/*===========================================================================

FUNCTION tdsrrcdt_init_procedure

DESCRIPTION
  This function initializes everything that the Direct Transfer procedures
  needs to have initialized.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcdt_init_procedure(void)

{
  uint8 count;

  for (count = 0; count <MAX_NAS_MESSAGES_FOR_CS_DOMAIN; count++ )
  {
    tdsrrcudt_cs_saved_data[count].mui_status = FALSE;
    tdsrrcudt_cs_saved_data[count].data_req_status = FALSE;
  }

  for (count = 0; count <TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN; count++ )
  {
    tdsrrcudt_ps_saved_data[count].mui_status = FALSE;
    tdsrrcudt_ps_saved_data[count].data_req_status = FALSE;
  }

  for (count = 0; count < (MAX_NAS_MESSAGES_FOR_CS_DOMAIN + 
                           TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN); count++)
  {
    tdsrrcddt_saved_data[count].data_ind_status = FALSE;
  }

  /* Initialize close_session req flag for PS/CS domain to false  */
  tdsrrcscrr_saved_cs_close_session_req.rrc_sig_con_rel_status = FALSE;
  tdsrrcscrr_saved_cs_close_session_req.close_sess_req_status = FALSE;
  tdsrrcscrr_saved_cs_close_session_req.mui_status = TDSRRCSCRR_MUI_STATUS_FALSE;
  tdsrrcscrr_saved_cs_close_session_req.rrc_proc= TDSRRC_PROCEDURE_NONE;

  tdsrrcscrr_saved_ps_close_session_req.rrc_sig_con_rel_status = FALSE;
  tdsrrcscrr_saved_ps_close_session_req.close_sess_req_status = FALSE;
  tdsrrcscrr_saved_ps_close_session_req.mui_status = TDSRRCSCRR_MUI_STATUS_FALSE;
  tdsrrcscrr_saved_ps_close_session_req.rrc_proc = TDSRRC_PROCEDURE_NONE;

  /* Initialize open_session_req flag for PS/CS domain to false  */
  tdsrrcidt_cs_saved_open_session_req.mui_status = FALSE;
  tdsrrcidt_cs_saved_open_session_req.open_sess_req_status = FALSE;
  
  tdsrrcidt_ps_saved_open_session_req.mui_status = FALSE;
  tdsrrcidt_ps_saved_open_session_req.open_sess_req_status = FALSE;

  /* variables which store the establishment cause received from NAS */
  tdsrrcidt_cs_saved_open_session_req.est_cause=RRC_EST_TERM_UNKNOWN;
  tdsrrcidt_ps_saved_open_session_req.est_cause=RRC_EST_TERM_UNKNOWN;

  /* Initialize the Initial Direct Transfer Procedure variable
   which keeps track whether CU intiated in CELL/URA PCH*/
  tdsrrcidt_cs_saved_open_session_req.cu_init_by_idt = FALSE;
  tdsrrcidt_ps_saved_open_session_req.cu_init_by_idt = FALSE;

  /* Initialize the Initial Direct Transfer Procedure variable
   which keeps track of fresh START value need to be carried in IDT*/
  tdsrrcidt_cs_saved_open_session_req.fresh_start_value_needed = FALSE;
  tdsrrcidt_ps_saved_open_session_req.fresh_start_value_needed = FALSE;
  
  tdsrrcidt_cs_saved_open_session_req.open_session_req.call_type = RRC_CALL_TYPE_NONE;
  
  /* Initialize the Initial Direct Transfer Procedure variable
   which keeps track Selected PLMN I.D*/
  tdsrrcidt_cs_saved_open_session_req.selected_plmn_id.mcc[0]= 0;
  tdsrrcidt_cs_saved_open_session_req.selected_plmn_id.mcc[1]= 0;
  tdsrrcidt_cs_saved_open_session_req.selected_plmn_id.mcc[2]= 0;
  tdsrrcidt_cs_saved_open_session_req.selected_plmn_id.mnc[0]= 0;
  tdsrrcidt_cs_saved_open_session_req.selected_plmn_id.mnc[1]= 0;
  tdsrrcidt_cs_saved_open_session_req.selected_plmn_id.mnc[2]= 0;
  tdsrrcidt_cs_saved_open_session_req.selected_plmn_id.num_mnc_digits = 0;
  tdsrrcidt_ps_saved_open_session_req.selected_plmn_id = 
      tdsrrcidt_cs_saved_open_session_req.selected_plmn_id;
  tdsrrcidt_est_cause_info.valid = FALSE;
  tdsrrcidt_est_cause_info.est_cause = RRC_EST_TERM_UNKNOWN;
  tdsrrcidt_est_cause_info.to_be_sent_by=TDSRRC_PROCEDURE_NONE;
  tdsrrcidt_est_cause_info.last_proc_sent = TDSRRC_PROCEDURE_NONE;

  tdsrrcidt_est_cause_info.call_type = RRC_CALL_TYPE_NONE;


  /* Initialize the flag to show that a signalling session is not open to
   * the CS domain
   */
  tdsrrcdt_int_data.cs_domain_open = FALSE;

  /* Initialize the flag to show that a signalling session is not open to
   * the PS domain
   */
  tdsrrcdt_int_data.ps_domain_open = FALSE;

  /* Initialize the Initial Direct Transfer Procedure substate
   */ 
  tdsrrcidt_substate = TDSRRCIDT_INITIAL;

  /* Initailize the Uplink Direct Transfer Procedure substate 
  */
  tdsrrcudt_substate = TDSRRCUDT_INITIAL;
  /* Initial Signalling Connection Release Request Procedure substate
   */
  tdsrrcscrr_substate = TDSRRCSCRR_INITIAL;

  /* Register with the State Change Manager for all state transitions 
   * from any Connected state to Idle Disconnected state for
   * Initial Direct Transfer Procedure.*/
  tdsrrcscmgr_register_for_scn
  (
    TDSRRC_PROCEDURE_IDT,   
    TDSRRC_STATE_WILDCARD,
    TDSRRC_STATE_DISCONNECTED
  );

  /* Register with the State Change Manager for all state transitions 
   * from any Connected state to Idle Disconnected state for
   * Uplink Direct Transfer Procedure.*/
  tdsrrcscmgr_register_for_scn
  (
    TDSRRC_PROCEDURE_UDT,   
    TDSRRC_STATE_WILDCARD,
    TDSRRC_STATE_DISCONNECTED
  );

  /* Register with the State Change Manager for all state transitions 
   * from any Connected state to Idle Disconnected state for
   * Signalling Connection Release Request Procedure.*/
  tdsrrcscmgr_register_for_scn
  (
    TDSRRC_PROCEDURE_SCRR,
    TDSRRC_STATE_WILDCARD,
    TDSRRC_STATE_DISCONNECTED
  );

  /* Register with the State Change Manager for all state transitions 
   * from any Connected state to Idle Disconnected state for
   * Downlink Direct Transfer Procedure.*/
  tdsrrcscmgr_register_for_scn
  (
    TDSRRC_PROCEDURE_DDT,
    TDSRRC_STATE_WILDCARD,
    TDSRRC_STATE_DISCONNECTED
  );
} /* tdsrrcdt_init_procedure */

/*===========================================================================

FUNCTION tdsrrcdt_check_if_emergency_call_initiated

DESCRIPTION
  This function checks whether emergency call has been initiated for Initial Direct Transfer and Uplink Direct Transfer

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: Emergency call is initiated
  FALSE: Otherwise

===========================================================================*/
boolean tdsrrcdt_check_if_emergency_call_initiated()
{
  boolean status=FALSE;
  int i;

  /*If IDT is enqueued in RLC or buffered at DT procedure, return TRUE*/
  if(((tdsrrcidt_cs_saved_open_session_req.mui_status== TRUE) ||
     (tdsrrcidt_cs_saved_open_session_req.open_sess_req_status == TRUE))
     &&
     (tdsrrcidt_cs_saved_open_session_req.est_cause==RRC_EST_EMERGENCY_CALL)
    )
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"IDT is in process for Ecall mui_status=%d,open_sess_req_status =%d, est_cause =%d",tdsrrcidt_cs_saved_open_session_req.mui_status,
                                                                                  tdsrrcidt_cs_saved_open_session_req.open_sess_req_status,
                                                                                  tdsrrcidt_cs_saved_open_session_req.est_cause);
    return (TRUE);
  }
  /*Check to see if is_emergency_call is set in any any UDT message*/
  if(tdsrrcudt_substate==TDSRRCUDT_WAIT_FOR_L2ACK)
  {
    for(i=0; i < MAX_NAS_MESSAGES_FOR_CS_DOMAIN ;i++)
    {
      if(((tdsrrcudt_cs_saved_data[i].mui_status == TRUE) ||
         (tdsrrcudt_cs_saved_data[i].data_req_status == TRUE))
         &&
         (tdsrrcudt_cs_saved_data[i].data_req.is_emergency_call == TRUE)
        )
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"UDT is in process for Ecall mui_status=%d,data_req_status =%d, is_emergency_call =%d",tdsrrcudt_cs_saved_data[i].mui_status,
                                                                                  tdsrrcudt_cs_saved_data[i].data_req_status,
                                                                                  tdsrrcudt_cs_saved_data[i].data_req.is_emergency_call);
        return (TRUE);
      }
    }
  }
  return status;
}/*tdsrrcdt_check_if_emergency_call_initiated*/


/*==========================================================================

FUNCTION tdsrrcdt_fill_plmn_id_in_idt

DESCRIPTION
  This function will fill's selected PLMN I.D in
  IDT message.
  .
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcdt_fill_plmn_id_in_idt
(
  /* PLMN I.D to be sent over the air */
  tdsrrc_PLMN_Identity *plmn_id_msg_ie,
  /*CN Domain I.Dfor which PLMN I.D to be filled*/
  rrc_cn_domain_identity_e_type cn_id
)
{
  uint32 i, n_index;                            /* Temporary variables for indexing */    
  rrc_plmn_identity_type slelected_plmn_id;     /* Temporary varibale to update linked list members */       
  tdsrrc_MCC *temp_mcc = NULL;
  tdsrrc_MNC *temp_mnc = NULL;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Fill PLMN I.D from CN I.D= %d",cn_id);
  /*Get the slelected_plmn_id from the stored PLMN depending on CN domain*/
  if(cn_id == RRC_CS_DOMAIN_CN_ID)
  {
    slelected_plmn_id = tdsrrcidt_cs_saved_open_session_req.selected_plmn_id;
  }
  else if(cn_id == RRC_PS_DOMAIN_CN_ID)
  {
    slelected_plmn_id = tdsrrcidt_ps_saved_open_session_req.selected_plmn_id;
  }
  else
  {
    /*This is error handling, whic will not be hit as there are many checks
      in open_session_req handling  */
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Wrong CN I.D passed %d,Fill CS Domain PLMN",cn_id);
    slelected_plmn_id = tdsrrcidt_cs_saved_open_session_req.selected_plmn_id;
  }

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS:Include camped PLMN I.D in IDT");

  tdsrrc_print_plmn_id(slelected_plmn_id);

  /*Check for number of digits in MNC either its 2 or 3 & then  print */
  if((slelected_plmn_id.num_mnc_digits != 3)&&
    (slelected_plmn_id.num_mnc_digits != 2))
  {
    /*Lets set the default value to 3, This is error handling,
    whic will not be hit as NAS takes care of setting number of 
    MNC digits to either 2 or 3. This check is good since it directly 
    effects ASN.1 encoding if this feild is set to more than 3*/
    slelected_plmn_id.num_mnc_digits = 3;
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"NWS:Invalid Number of MNC digits = %d",slelected_plmn_id.num_mnc_digits);
  }

  /* Update the pointer to the linked list */
    temp_mcc =  &(plmn_id_msg_ie->mcc);

  temp_mcc->n = RRC_MAX_MCC_DIGITS;
  n_index = 0;
  ALLOC_ASN1ARRAY1(&tdsenc_ctxt,temp_mcc,tdsrrc_Digit);
 

  if(temp_mcc->elem == NULL)
  {
    ERR_FATAL("Memory allocation failed",0,0,0);
  }

  for(i = 0;i < RRC_MAX_MCC_DIGITS;i++)
  {
    temp_mcc->elem[n_index] = slelected_plmn_id.mcc[i];
    n_index++;
  }

  /* Update the pointer to the linked list */
  temp_mnc = &(plmn_id_msg_ie->mnc);
  temp_mnc->n = slelected_plmn_id.num_mnc_digits;
  n_index = 0;
  
  ALLOC_ASN1ARRAY1(&tdsenc_ctxt,temp_mnc,tdsrrc_Digit);
  if(temp_mnc->elem == NULL)
  {
    ERR_FATAL("Memory allocation failed",0,0,0);
  }
  for(i = 0;i < slelected_plmn_id.num_mnc_digits;i++)
  {
    temp_mnc->elem[n_index] = slelected_plmn_id.mnc[i];
    n_index++;
  }
}

#if defined(FEATURE_3GPP_FAST_DORMANCY)
/*===========================================================================

FUNCTION tdsrrcdt_update_scrr_to_handle_dormancy

DESCRIPTION
  This function handles TDSRRC_CLOSE_SESSION_REQ and/or TDSRRC_SIG_CONN_RELEASE_REQ in case 
  TDSRRC_FAST_DORMANCY_REQ is active. 
  It updates the TDSRRC_CLOSE_SESSION_REQ and/or TDSRRC_SIG_CONN_RELEASE_REQ database
  and aborts TDSRRC_FAST_DORMANCY_REQ.

  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE: Emergency call is initiated
  FALSE: Otherwise

===========================================================================*/
boolean tdsrrcdt_update_scrr_to_handle_dormancy
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
)
{
  boolean status = FALSE;
  
  if((TDSRRC_CLOSE_SESSION_REQ == cmd_ptr->cmd_hdr.cmd_id) ||
     (TDSRRC_SIG_CONN_RELEASE_REQ ==  cmd_ptr->cmd_hdr.cmd_id))
  {
    if((TRUE == tdsrrcdormancy_check_dormancy_initiated_scrr_active()) &&
       (RRC_PS_DOMAIN_CN_ID == cmd_ptr->cmd.close_sess_req.cn_domain_id))
    {
      if(!((TDSRRC_SIG_CONN_RELEASE_REQ ==  cmd_ptr->cmd_hdr.cmd_id) &&
          (TDSRRC_PROCEDURE_DORMANCY == cmd_ptr->cmd.rrc_sig_conn_req.rrc_proc))
         )
      {
        /* Update CLOSE_SESSION or SIG_CONN_RELEASE database to act as if SCRI is sent because of them */
        switch(tdsrrcscrr_substate)
        {
          case TDSRRCSCRR_INITIAL:
            /* No need to do anything. Dorrmancy will be aborted and let CLOSE_SESSION
               * or SIG_CONN_REL go on. */
            break;

          case TDSRRCSCRR_WAIT_FOR_CELL_UPDATE_COMP_CNF:
            /* Dormancy initiated SCRI is waiting for CU_COMP_CNF
               * Update as if CLOSE_SESSION or SIG_CONN_REL does that */
            if(TDSRRC_CLOSE_SESSION_REQ == cmd_ptr->cmd_hdr.cmd_id)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Mapping Dormancy initiated SCRI to CLOSE_SESSION");
              tdsrrcscrr_saved_ps_close_session_req.close_sess_req_status = TRUE;
            }
            
            if(TDSRRC_SIG_CONN_RELEASE_REQ ==  cmd_ptr->cmd_hdr.cmd_id)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Mapping Dormancy initiated SCRI to SIG_CONN_REL");        
              tdsrrcscrr_saved_ps_close_session_req.rrc_sig_con_rel_status = TRUE;
            }
            tdsrrcscrr_saved_ps_close_session_req.mui_status = TDSRRCSCRR_MUI_STATUS_FALSE;
            status = TRUE;
            break;
            
          case TDSRRCSCRR_WAIT_FOR_L2ACK:
            if(TDSRRC_CLOSE_SESSION_REQ == cmd_ptr->cmd_hdr.cmd_id)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Mapping Dormancy initiated SCRI to CLOSE_SESSION");
              tdsrrcscrr_saved_ps_close_session_req.mui_status = TDSRRCSCRR_MUI_CLOSE_SESSION_CNF;
              tdsrrcscrr_saved_ps_close_session_req.close_sess_req_status = FALSE;  
            }
            
            if(TDSRRC_SIG_CONN_RELEASE_REQ ==  cmd_ptr->cmd_hdr.cmd_id)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Mapping Dormancy initiated SCRI to SIG_CONN_REL");     
              tdsrrcscrr_saved_ps_close_session_req.mui_status = TDSRRCSCRR_MUI_SIG_CON_REL_CNF;
              tdsrrcscrr_saved_ps_close_session_req.rrc_sig_con_rel_status = FALSE;              
              tdsrrcscrr_saved_ps_close_session_req.rrc_proc = cmd_ptr->cmd.rrc_sig_conn_req.rrc_proc;
            }
            status = TRUE;
            break;
        }

        /* PS domain is anyway getting closed by SCRI */    
        tdsrrcdormancy_check_and_abort_dormancy_req(RRC_FAST_DORMANCY_SUCCESS_APP_NO_RETRY);    
      }
    }
    else
    {
      tdsrrcdormancy_check_and_abort_dormancy_req(RRC_FAST_DORMANCY_SUCCESS_APP_NO_RETRY);    
    }
  }

  return status;
}
#endif 
#ifdef FEATURE_LTE_TO_TDSCDMA
/*===========================================================================

FUNCTION tdsrrcdt_check_n_send_sync_ind_for_ps_to_fake_security

DESCRIPTION
  This function sends SYNC_IND for PS domain for integrity
  protection
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE: Emergency call is initiated
  FALSE: Otherwise

===========================================================================*/
void tdsrrcdt_check_and_send_sync_ind_for_ps_to_fake_security(void)
{
 if(tdsnv_fake_security_enabled)
 {
   /* For PS send the RRC_SYNC_IND for Integrity Protection */
   tdsrrcidt_send_sync_ind(RRC_INTEGRITY_ESTABLISHED, RRC_PS_DOMAIN_CN_ID);
 }
}
#endif
/*===========================================================================

FUNCTION tdsrrcudt_process_l2_ack_cell_udpate_state

DESCRIPTION
   This function is used to handle L2 ack when UDT at cell update state.
   if L2 ACK success, follow non cell udpate state design.
   if L2 ACK failure, reset the status and wait for cell udpate compelete.
  
DEPENDENCIES
  None.

RETURN VALUE
  void

===========================================================================*/
void tdsrrcudt_process_l2_ack_cell_udpate_state(tdsrrc_cmd_type *cmd_ptr )
{
  uint8 count_index;                    /* Count for saved messages */

  uint8               temp_mui = 0;
  tdsrrc_RB_Identity rb_id;                /* Local variable to store RB Id */
  rrc_nas_msg_priority_e_type nas_msg_priority;
  boolean           msg_found = FALSE; 
  
  
  rb_id = tdsrrclcm_get_ul_rb_id(cmd_ptr->cmd.downlink_l2ack_ind.lc_id);

  if((rb_id < DCCH_DT_HIGH_PRI_RADIO_BEARER_ID) || 
     (rb_id > DCCH_DT_LOW_PRI_RADIO_BEARER_ID))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Can't find the LC ID");
    return;
  }

  if(rb_id == DCCH_DT_HIGH_PRI_RADIO_BEARER_ID)
  {
    nas_msg_priority = RRC_NAS_MSG_HIGH_PRIO; 
  }
  else
  {
    nas_msg_priority = RRC_NAS_MSG_LOW_PRIO;
  }
  /* Check for the MUI received */
  temp_mui = cmd_ptr->cmd.downlink_l2ack_ind.mui;
  if (SUCCESS == cmd_ptr->cmd.downlink_l2ack_ind.status)
  {
    if(SUCCESS == tdsrrcudt_delete_saved_data_req((rrc_nas_msg_priority_e_type) nas_msg_priority,
                                                    temp_mui))
    {
      
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unexpected L2 ACK rcvd");
      /* Do nothing */
    }    
  }
  else
  {          
    for ( count_index = 0; count_index < MAX_NAS_MESSAGES_FOR_CS_DOMAIN; count_index++)
    {
      if( (tdsrrcudt_cs_saved_data[count_index].mui == temp_mui) &&
          (tdsrrcudt_cs_saved_data[count_index].data_req.priority == nas_msg_priority) &&
          (tdsrrcudt_cs_saved_data[count_index].mui_status == TRUE)
          )
      {
        //force data_req_status to true bcos we want to resend this again
        tdsrrcudt_cs_saved_data[count_index].data_req_status = TRUE;
        tdsrrcudt_cs_saved_data[count_index].mui_status = FALSE;
        msg_found = TRUE;
        break;
      }
    }
    if(msg_found != TRUE)
    {
      for ( count_index = 0; count_index < TDSMAX_NAS_MESSAGES_FOR_PS_DOMAIN; count_index++)
      {
        if( (tdsrrcudt_ps_saved_data[count_index].mui == temp_mui) &&
            (tdsrrcudt_ps_saved_data[count_index].data_req.priority == nas_msg_priority) &&
            (tdsrrcudt_ps_saved_data[count_index].mui_status == TRUE)
            )
        {
          //force data_req_status to true bcos we want to resend this again
          tdsrrcudt_ps_saved_data[count_index].data_req_status = TRUE;
          tdsrrcudt_ps_saved_data[count_index].mui_status = FALSE;
          break;
        }
      }
    }
  }

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Keep udt at wait cell udpate complete state.");  
  return;

}

/*===========================================================================

FUNCTION tdsrrcdt_is_cs_voice_call_domain_open

DESCRIPTION
   This function is used to indicate TDS is in cs voice call or not.
   if TRUE,TDS is in cs domain and voice call
   else return FALSE
  
DEPENDENCIES
  None.

RETURN VALUE
  void

===========================================================================*/
boolean tdsrrcdt_is_cs_voice_call_domain_open()
{
  if ((SYS_PROC_TYPE_CS_CALL == tdsrrc_proc_type)&&tdsrrcdt_is_cs_domain_open())
  {    
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/*===========================================================================

FUNCTION tdsrrcdt_is_cs_sms_call_domain_open

DESCRIPTION
   This function is used to indicate TDS is in cs sms call or not.
   if TRUE,TDS is in cs domain and sms call
   else return FALSE
  
DEPENDENCIES
  None.

RETURN VALUE
  void

===========================================================================*/
boolean tdsrrcdt_is_cs_sms_call_domain_open()
{
  if ((SYS_PROC_TYPE_SMS == tdsrrc_proc_type)&&tdsrrcdt_is_cs_domain_open())
  {    
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

