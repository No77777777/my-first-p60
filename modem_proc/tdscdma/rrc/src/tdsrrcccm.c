/*===========================================================================
         R R C    C E L L    C H A N G E    M A N A G E R    
DESCRIPTION

   This file contains Cell Change Manager (CCM). CCM is responsible for the 
   following -
    1. Carry out steps to transition to a new cell or no cell when a request
       is made by any RRC procedure.
    2. Maintain the current cell camp status. 

   Camp Status
   ======================
   It indicates if UE is at present camped on a Cell or is in Cell Loss condition. 
   When the status indicates Camping on a cell, all SIBs for that cell can be 
   presumed to be available.
   CCM maintains Camp status based on Cell Change Requests that come when a New
   Cell is selected or when a Cell Loss occurs without New Cell selection.
   CCM invalidates the Camp status during the time it processes the Cell Change 
   Request. This has the effect that till Cell Change Request is fully processed,
   UE is presumed to be Not Camping on any cell.




EXTERNALIZED FUNCTIONS   
   
      
INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcccm.c#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/25/11   ysh      Modify req_mak in cphy_setup_req.
10/13/11   mkh     Added log version to RRC log packets
08/16/11   gx      change marco to functions
08/11/11   gx       rename functions from tdsrrc_ccm_XXX to tdsrrcccm_XXX
07/07/11   mkh     Renamed log pkts to TDSxxx
03/10/11   yzh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/


/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"
#include "rex.h"
#include "msg.h"
#include "err.h"
#include "tdsuecomdef.h"

#include "tdsrrcmm_v.h"
#include "tdsrrcintif.h"
#include "tdsrrccmd_v.h"

#include "tdsrrccspdb.h"
#include "tdsrrccspi.h"
#include "tdsrrcccm.h"
#include "tdsrrclcm.h"
#include "tdsrrcllc.h"
#include "tdsrrcsibproc.h"
#include "tdsrrcsibdb.h"

/* Needed for calling procedures when propagating New Cell Ind. */
#include "tdsrrccsp.h"
#include "tdsrrcrbe.h"
#include "tdsrrcrce.h"
#include "tdsrrcscmgr.h"
#include "tdsrrccu.h"

#include "tdsrrcrbr.h"
#include "tdsrrcpcreconfig.h"
#include "tdsrrcrbreconfig.h"
#include "tdsrrctcreconfig.h"
#include "tdsrrcllcoc.h"
#include "tdsrrcdata.h"
#include "tdsrrccommon.h"
#include "tdsrrcpg1.h"
#ifdef T_WINNT
#error code not present
#endif

#include "tdsrrclogging.h"

#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "tdsCFAParser_i.h"
#include "tdsrrcnv.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */

/* This holds the current cell camping info for the UE */
/* It is also used by LLC */
tdsrrc_ccm_cell_camping_info_type   tdsrrc_ccm_cell_camping_info;
/* This holds the Substate for RRC CCM */
/*This variable is used in file scope only and set for global for debugging purpose only*/
tdsrrc_ccm_substate_e_type      tdsrrc_ccm_substate;

rrc_service_ind_type tdsrrc_service_ind_backup;

extern rrc_cn_domain_identity_e_type     tdsrrc_est_cn_domain_id;
extern tdsrrc_cs_domain_error_count_type tdsrrc_cs_domain_error;


extern tdsrrcpg1_page_detect_pattern_type tdsrrcpg1_page_detect_pattern;


uecomdef_status_e_type  
tdsrrcccm_send_new_cell_ind(tdsrrc_new_cell_status_e_type  new_cell_camp_status,
                          tdsrrc_proc_e_type  proc_id,
                          boolean cell_same_as_before,
                          boolean inform_procedure_rce);

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

FUNCTION          RRC_CCM_GET_CURR_CAMPING_STATUS

DESCRIPTION       This function is responsible for returning the current 
                  Camping Status of the UE.

DEPENDENCIES
                  None.

RETURN VALUE
                  The Current Camping Status as maintained by Cell Change Mgr.

SIDE EFFECTS      None.
===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/

tdsrrc_camped_on_status_e_type  
tdsrrcccm_get_curr_camping_status(tdsrrc_proc_e_type req_proc_id)
{
  /* Cell camping status in invalid either because we are about to camp on 
   * a cell or a Reselection is in progress
   */
  if((tdsrrc_ccm_cell_camping_info.status == TDSSTATUS_INVALID_CELL_TO_BE_CAMPED)||
     (tdsrrc_ccm_cell_camping_info.status == TDSSTATUS_INVALID_CELL_RESELECTION))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Curr Camping Status Not Valid,return %d",TDSRRC_NOT_CAMPED_ON);
    if(req_proc_id == TDSRRC_PROCEDURE_RCE)
    {
      tdsrrc_ccm_cell_camping_info.rce_found_status_not_camping = TRUE;
    }
    return(TDSRRC_NOT_CAMPED_ON);
  }
  else  /* Camping status is valid */
  {
    if(tdsrrc_ccm_cell_camping_info.cell_camp_status == TDSRRC_NEW_CELL_SELECTED)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Return Curr Camping Status %d",TDSRRC_CAMPED_ON);
      if(req_proc_id == TDSRRC_PROCEDURE_RCE)
      {
        tdsrrc_ccm_cell_camping_info.rce_found_status_not_camping = FALSE;
      }
      return(TDSRRC_CAMPED_ON);
    }
    else if(tdsrrc_ccm_cell_camping_info.cell_camp_status == TDSRRC_CELL_LOSS_NO_NEW_CELL)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Return Curr Camping Status %d",TDSRRC_NOT_CAMPED_ON);
      if(req_proc_id == TDSRRC_PROCEDURE_RCE)
      {
        tdsrrc_ccm_cell_camping_info.rce_found_status_not_camping = TRUE;
      }
      return(TDSRRC_NOT_CAMPED_ON);
    }
    else
    {
      /* This is Invalid value - something wrong */
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"Invalid stored Camp Status %d,return Status %d",tdsrrc_ccm_cell_camping_info.cell_camp_status,TDSRRC_NOT_CAMPED_ON);
      
      if(req_proc_id == TDSRRC_PROCEDURE_RCE)
      {
        tdsrrc_ccm_cell_camping_info.rce_found_status_not_camping = TRUE;
      }
      return(TDSRRC_NOT_CAMPED_ON);
    }
  }  /* Camping status is valid */

} /* tdsrrcccm_get_curr_camping_status */

/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

/* =======================================================================
**                            Function Definitions
** ======================================================================= */


/*===========================================================================

FUNCTION          tdsrrcccm_copy_curr_cell_camping_info 

DESCRIPTION       This function  copies the contents of Cell Change Request to Current Cell data in
                  tdsrrc_ccm_cell_camping_info.
  
DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None

===========================================================================*/
static void tdsrrcccm_copy_curr_cell_camping_info
(
  tdsrrc_cmd_type  *cmd_ptr
)
{
  tdsrrc_ccm_cell_camping_info.curr_cell.freq = cmd_ptr->cmd.cell_change_req.freq;
  tdsrrc_ccm_cell_camping_info.curr_cell.cpid = cmd_ptr->cmd.cell_change_req.cpid;
  tdsrrc_ccm_cell_camping_info.curr_cell.plmn_id = cmd_ptr->cmd.cell_change_req.plmn_id;
  tdsrrc_ccm_cell_camping_info.curr_cell.cell_id =  cmd_ptr->cmd.cell_change_req.cell_id;
  tdsrrc_ccm_cell_camping_info.curr_cell_access = cmd_ptr->cmd.cell_change_req.new_cell_access;

/* If FEATURE_DSAC is defined get the cell access information for the CS and PS domain(s) */
#ifdef FEATURE_DSAC
  tdsrrc_ccm_cell_camping_info.curr_cs_cell_access = cmd_ptr->cmd.cell_change_req.new_cs_cell_access;
  tdsrrc_ccm_cell_camping_info.curr_ps_cell_access = cmd_ptr->cmd.cell_change_req.new_ps_cell_access;
  tdsrrc_ccm_cell_camping_info.curr_dsac_present= cmd_ptr->cmd.cell_change_req.dsac_present;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSAC: DSAC value at cell change req =%d {0=Not Present/1=Present}",tdsrrc_ccm_cell_camping_info.curr_dsac_present);
#endif

}


/*===========================================================================

FUNCTION          RRC_CCM_UPDATE_WITH_CELL_LOSS 

DESCRIPTION       This function updates the Camping status in CCM while 
                  maintaining the CCM state.
  
DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrcccm_update_with_cell_loss(void)
{
 if((tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
     && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN))
  {
    TDSRRC_MSG1(MSG_LEGACY_MED,"Updating CCM with cell loss. Scan type is %d",tdsrrc_csp_int_data.curr_scan);

    /* First copy current cell to previous cell since we are ready to accept
    * next cell change request.
    */
    tdsrrc_ccm_cell_camping_info.prev_cell = 
      tdsrrc_ccm_cell_camping_info.curr_cell;

    tdsrrc_ccm_cell_camping_info.prev_cell_access = 
        tdsrrc_ccm_cell_camping_info.curr_cell_access;


#ifdef FEATURE_DSAC
  tdsrrc_ccm_cell_camping_info.prev_dsac_present = 
    tdsrrc_ccm_cell_camping_info.curr_dsac_present;

  tdsrrc_ccm_cell_camping_info.prev_cs_cell_access = 
    tdsrrc_ccm_cell_camping_info.curr_cs_cell_access;

  tdsrrc_ccm_cell_camping_info.prev_ps_cell_access = 
    tdsrrc_ccm_cell_camping_info.curr_ps_cell_access;   
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSAC: Back up of DSAC info during  ccm update with cell loss is done !");
#endif 

  tdsrrc_ccm_cell_camping_info.req_proc_id = TDSRRC_PROCEDURE_CSP;

  tdsrrc_ccm_cell_camping_info.cell_camp_status = TDSRRC_CELL_LOSS_NO_NEW_CELL;

  tdsrrc_ccm_substate = TDSRRC_CCM_IDLE;
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_MED,"Not updating CCM with cell loss. Scan type is %d",tdsrrc_csp_int_data.curr_scan);
  }  
    
} /* tdsrrcccm_update_with_cell_loss */


/*===========================================================================

FUNCTION          RRC_CCM_LOG_CELL_INFO 

DESCRIPTION       This function will log the current cell information. This function
                  should only be called when UE is camped, i.e, this function
                  should not be called immediately after reading SIBs, etc. 

  
DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None

===========================================================================*/
static void tdsrrcccm_log_cell_info( void)
{  

#if (!defined T_WINNT)

  /* Do QXDM logging here */
  uint32 log_record_length;                 /* Local variable to store Log 
                                               Record Length */
  /* Local variable to store Log Record */
  tdscdma_rrc_cell_id_log_packet_type *log_record_ptr;
  tdsrrc_int_ura_id_type ura_id = 0;

#ifdef FEATURE_DSAC
  mm_as_cell_access_status_T  cell_access;
#endif
  tdsrrc_SysInfoType2 *sib2_ptr;
  tdsrrc_SysInfoType3 *sib3_ptr;
  tdsrrc_SysInfoType4 *sib4_ptr;
  tdsrrc_CellAccessRestriction *sib3_4_access_restriction_ptr;

  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  rrc_plmn_identity_type plmn_id;
  if(tdsrrc_ccm_cell_camping_info.curr_cell.is_valid_nw_sharing)
  {
    plmn_id = tdsrrc_ccm_cell_camping_info.curr_cell.shared_list_of_plmns.common_plmn_id;
  }
  else
  {
    plmn_id = tdsrrc_ccm_cell_camping_info.curr_cell.plmn_id;
  }

  sib2_ptr = (tdsrrc_SysInfoType2 *) tdsrrc_sibdb_return_sib_for_cell(
                            tdsrrc_SIB2, 
                            plmn_id, 
                            tdsrrc_ccm_cell_camping_info.curr_cell.cell_id);

  sib3_ptr = (tdsrrc_SysInfoType3 *) tdsrrc_sibdb_return_sib_for_cell(
                            tdsrrc_SIB3, 
                            plmn_id, 
                            tdsrrc_ccm_cell_camping_info.curr_cell.cell_id);

  /*Calculate the record length 
    the 1 is added to calculate the length of call_access which is uint8*/
  /*lint -e413*/
  log_record_length = FPOS(tdscdma_rrc_cell_id_log_packet_type, call_access) + 1;
  /*lint +e413*/
  
  /*Allocate memory for Log Record. */
  log_record_ptr = (tdscdma_rrc_cell_id_log_packet_type *)log_alloc( TDSCDMA_RRC_CELL_ID_LOG_PACKET, log_record_length);
      
  if( sib3_ptr == NULL )
  {
    /* SIB3 for this PLMN, CEll ID combination not present */
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"SIB3 for MCC %d, MNC %d, Cell ID %d not present",
          TDSRRC_CSP_GET_INT_MCC(plmn_id), 
          TDSRRC_CSP_GET_INT_MNC(plmn_id), 
          tdsrrc_ccm_cell_camping_info.curr_cell.cell_id);

    if(log_record_ptr != NULL)
    {
      log_record_ptr->version = TDSRRC_LOG_VERSION;
      log_record_ptr->dl_uarfcn = log_record_ptr->ul_uarfcn = log_record_ptr->cell_id = 0;
      log_record_ptr->ura_id = 0;
      log_record_ptr->cell_access_rest = 0xF;
      TDSRRC_MSG0(MSG_LEGACY_MED,"Logging TDSCDMA_RRC_CELL_ID_LOG_PACKET");

      /*commit the log record */
      log_commit(log_record_ptr);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"diag is not enabled");
    } 

    return;
  } /* SIB3 not present */

  if((rrc_state != TDSRRC_STATE_DISCONNECTED) && (rrc_state != TDSRRC_STATE_CONNECTING))
  {
    if(sib3_ptr->sib4indicator == TRUE)
    {
      /* Read SIB4. If it's not there mark and error and continue with SIB3 */
      sib4_ptr = (tdsrrc_SysInfoType4 *) tdsrrc_sibdb_return_sib_for_cell(
                                tdsrrc_SIB4, 
                                plmn_id, 
                                tdsrrc_ccm_cell_camping_info.curr_cell.cell_id);
      if( sib4_ptr == NULL )
      {
        /* SIB4 not present */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB4 indicated but not present,using SIB3");
        sib3_4_access_restriction_ptr = &(sib3_ptr->cellAccessRestriction);
      }
      else
      {
        sib3_4_access_restriction_ptr = &(sib4_ptr->cellAccessRestriction);
      }
    }
    else
    {
      sib3_4_access_restriction_ptr = &(sib3_ptr->cellAccessRestriction);
    }
  }
  else
  {
    sib3_4_access_restriction_ptr = &(sib3_ptr->cellAccessRestriction);
  }

  /* Make sure memory allocation is successful before to proceed with 
  logging */
  if(log_record_ptr != NULL)
  {
    log_record_ptr->version = TDSRRC_LOG_VERSION;
    /* Update dl_uarfcn */
    log_record_ptr->dl_uarfcn = tdsrrc_ccm_cell_camping_info.curr_cell.freq;

    log_record_ptr->ul_uarfcn = tdsrrc_ccm_cell_camping_info.curr_cell.freq;
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"UL UARFCN : %d,DL UARFCN: %d",log_record_ptr->ul_uarfcn,log_record_ptr->dl_uarfcn);

    /* Update cell_id */
    log_record_ptr->cell_id = tdsrrc_ccm_cell_camping_info.curr_cell.cell_id;

    /* Update ura_id */
    if(sib2_ptr != NULL)
    {
      tdsrrc_translate_ura_id(sib2_ptr->ura_IdentityList.elem, &ura_id);
      log_record_ptr->ura_id = (uint16) ura_id;
    }
    else
    {
      log_record_ptr->ura_id = 0;
    } 

    /* Update cell_access_rest */
    log_record_ptr->cell_access_rest = 0;
    if(sib3_4_access_restriction_ptr->cellBarred.t == T_tdsrrc_CellBarred_barred)
    {
      log_record_ptr->cell_access_rest |= TDSRRCLOG_CELL_BARRED;
    }
    else
    {
      log_record_ptr->cell_access_rest |= TDSRRCLOG_CELL_NOT_BARRED;
    }

    if(sib3_4_access_restriction_ptr->cellReservedForOperatorUse == tdsrrc_ReservedIndicator_reserved)
    {
      log_record_ptr->cell_access_rest |= TDSRRCLOG_CELL_RESERVED;
    }
    else
    {
      log_record_ptr->cell_access_rest |= TDSRRCLOG_CELL_NOT_RESERVED;
    }

    if(sib3_4_access_restriction_ptr->cellReservationExtension == tdsrrc_ReservedIndicator_reserved)
    {
      log_record_ptr->cell_access_rest |= TDSRRCLOG_CELL_RESERVED_SOLSA;
    }
    else
    {
      log_record_ptr->cell_access_rest |= TDSRRCLOG_CELL_NOT_RESERVED_SOLSA;
    }

    if (tdsrrc_ccm_cell_camping_info.cell_camp_status == TDSRRC_NEW_CELL_SELECTED)
    {
      log_record_ptr->cell_access_rest |= TDSRRCLOG_UE_CAMPED;
    }
    else
    {
      log_record_ptr->cell_access_rest |= TDSRRCLOG_UE_NOT_CAMPED;
    }

    /* temporary structure to hold current cell access */
#ifdef FEATURE_DSAC
    cell_access =tdsrrc_ccm_cell_camping_info.curr_cell_access;
#endif

#ifdef FEATURE_DSAC
    if(tdsrrc_ccm_cell_camping_info.curr_dsac_present == TRUE)
    {
      if((tdsrrc_ccm_cell_camping_info.curr_cs_cell_access == MM_AS_ACCESS_ALL_CALLS)
         ||(tdsrrc_ccm_cell_camping_info.curr_ps_cell_access == MM_AS_ACCESS_ALL_CALLS))
      {
         cell_access = MM_AS_ACCESS_ALL_CALLS;
         log_record_ptr->call_access = RRCLOG_ALL_CALLS;
      }
      else
      {
        cell_access = MM_AS_ACCESS_EMERGENCY_ONLY;
        log_record_ptr->call_access = RRCLOG_EMERGENCY_CALLS;
      }
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSAC: CCM cell access for cell camping info",cell_access);
    }
#endif
    /* Update call_access */
    if ((tdsrrc_ccm_cell_camping_info.curr_cell_access == MM_AS_ACCESS_ALL_CALLS) ||
        (tdsrrc_ccm_cell_camping_info.curr_cell_access == MM_AS_ACCESS_NORMAL_ONLY))
    {
      log_record_ptr->call_access = RRCLOG_ALL_CALLS;
    }
    else
    {
      log_record_ptr->call_access = RRCLOG_EMERGENCY_CALLS;
    } 

    /*commit the log record */
    log_commit(log_record_ptr);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"diag is not enabled");
  }

#else
#error code not present
#endif /*QXDM_LOGGING */

} /* tdsrrcccm_log_cell_info */

/*===========================================================================
FUNCTION          RRCCCM_WAITING_FOR_RESELECTION_CNF

DESCRIPTION       This function returns a Boolean indicating if CCM is still
                  waiting for Cell Reselection Confirmation.
                  A State Change Indication could have put CCM to Idle substate
                  after it had sent the last Reselection Req. The function 
                  provides info to detect such an incidence.

DEPENDENCIES      None

RETURN VALUE      TRUE/FALSE

SIDE EFFECTS      None.
===========================================================================*/
boolean  tdsrrcccm_waiting_for_reselection_cnf(void)
{
  if(tdsrrc_ccm_substate == TDSRRC_CCM_WAIT_CELL_RESELECTION_CNF)
  {
    /* Still waiting for cell reselection */
    return(TRUE);
  }
  else
  {
    /* CCM could have been taken to Idle due to a State Change */
	TDSRRC_MSG0(MSG_LEGACY_MED,"CCM could have been taken to Idle due to a State Change");
    return(FALSE);
  }
} /* tdsrrcccm_waiting_for_reselection_cnf */

/*===========================================================================

FUNCTION          RRC_CCM_GET_CURR_CAMPED_FREQ

DESCRIPTION       This function is responsible for returning the current 
                  Camped on frequency of the UE.

DEPENDENCIES      Calling procedure needs to call 
                  tdsrrcccm_get_curr_camping_status before calling this function.
                  The returned value from this function is valid only if 
                  tdsrrcccm_get_curr_camping_status returns TDSRRC_CAMPED_ON

RETURN VALUE
                  Currently camped on frequency

SIDE EFFECTS      None.
===========================================================================*/
tdsrrc_freq_type tdsrrcccm_get_curr_camped_freq(void)
{
  return(tdsrrc_ccm_cell_camping_info.curr_cell.freq);
}

/*===========================================================================

FUNCTION          RRC_CCM_GET_CURR_CAMPED_PSC

DESCRIPTION       This function is responsible for returning the current 
                  Camped on Primary Scrambling Code of the UE.

DEPENDENCIES      Calling procedure needs to call 
                  tdsrrcccm_get_curr_camping_status before calling this function.
                  The returned value from this function is valid only if 
                  tdsrrcccm_get_curr_camping_status returns TDSRRC_CAMPED_ON

RETURN VALUE
                  Currently camped on Primary Scrambling Code

SIDE EFFECTS      None.
===========================================================================*/
tdsrrc_cpid_type tdsrrcccm_get_curr_camped_cpid(void)
{
  /* In TDD CPID and SCR are the same code */
  return(tdsrrc_ccm_cell_camping_info.curr_cell.cpid);
  
  /* ZZTBD: keep the original code as reference until we change the function and var names
   * return(TDSSCR_CODE_TO_PRI_SCR_CODE(tdsrrc_ccm_cell_camping_info.curr_cell.scr_code));
   */
}

/*===========================================================================

FUNCTION          RRC_CCM_GET_CURR_CAMPED_PLMN

DESCRIPTION       This function is responsible for returning the current 
                  Camped on PLMN of the UE.

DEPENDENCIES      NONE

RETURN VALUE
                  Currently camped on PLMN

SIDE EFFECTS      None.
===========================================================================*/
rrc_plmn_identity_type tdsrrcccm_get_curr_camped_plmn(void)
{
  return(tdsrrc_ccm_cell_camping_info.curr_cell.plmn_id);
}

/*===========================================================================

FUNCTION          RRC_CCM_GET_CURR_CAMPED_CELL_ID

DESCRIPTION       This function is responsible for returning the current 
                  Camped on cell id of the UE.

DEPENDENCIES      NONE

RETURN VALUE
                  Currently camped on cell

SIDE EFFECTS      None.
===========================================================================*/
tdsrrc_int_cell_id_type tdsrrcccm_get_curr_camped_cell_id(void)
{
  return(tdsrrc_ccm_cell_camping_info.curr_cell.cell_id);
}

/*===========================================================================

FUNCTION          RRC_CCM_GET_CURR_CELL_ACCESS_STATUS

DESCRIPTION       This function is responsible for returning the 
                  cell access status for the currently camped on cell.
                  If not camped on a cell, a cell access status of 
                  RRC_NO_CELL_ACCESS is returned.

DEPENDENCIES
                  None.

RETURN VALUE
              
SIDE EFFECTS      None.
===========================================================================*/

mm_as_cell_access_status_T
tdsrrcccm_get_curr_cell_access_status(void)
{
  if((tdsrrc_ccm_cell_camping_info.status != TDSSTATUS_VALID) ||
     (tdsrrc_ccm_cell_camping_info.cell_camp_status == TDSRRC_CELL_LOSS_NO_NEW_CELL)
     )
  {
    /* This is the case when we are not camped on any cell */
    return(MM_AS_ACCESS_NO_CALLS);
  }
  else
  {
    /* camped on a cell - return the current cell access status */
    return(tdsrrc_ccm_cell_camping_info.curr_cell_access);
  }
} /* tdsrrcccm_get_curr_cell_access_status */


/*===========================================================================

FUNCTION          RRC_CCM_GET_SERV_PLMN_CELLID

DESCRIPTION       This function is responsible for returning the PLMN and 
                  Cell ID of the serving cell.

DEPENDENCIES
                  None

RETURN VALUE
                  A status that indicates whether there is a serving cell 
                  or not..
SIDE EFFECTS
                  The returned status MUST be checked by the caller before
                  reading the PLMN or Cell ID. If returned status indicates
                  No Serving Cell, PLMN and Cell ID will not be initialized.

===========================================================================*/
tdsrrc_new_cell_status_e_type
tdsrrcccm_get_serv_plmn_cellid(tdsrrc_int_cell_id_type *cell_id, rrc_plmn_identity_type *plmn_id)
{
  if((tdsrrc_ccm_cell_camping_info.cell_camp_status != TDSRRC_NEW_CELL_SELECTED) ||
     (tdsrrc_ccm_cell_camping_info.status != TDSSTATUS_VALID))
  {
    return(TDSRRC_CELL_LOSS_NO_NEW_CELL);
  }
  else
  {
    *cell_id = tdsrrc_ccm_cell_camping_info.curr_cell.cell_id;
    *plmn_id = tdsrrc_ccm_cell_camping_info.curr_cell.plmn_id;
    return(TDSRRC_NEW_CELL_SELECTED);
  }
} /* tdsrrcccm_get_serv_plmn_cellid */

/*===========================================================================
FUNCTION          RRC_CCM_IS_RESELECTION_IN_PROGRESS

DESCRIPTION       This function is responsible for returning the current 
                  status of Reselection in progress.
DEPENDENCIES
                  None

RETURN VALUE
                  A status that indicates if a Reselection is in progress
                  or not.
                  
SIDE EFFECTS      None
===========================================================================*/
boolean  tdsrrcccm_is_reselection_in_progress(void)
{
  if(tdsrrc_ccm_cell_camping_info.status == TDSSTATUS_INVALID_CELL_RESELECTION)
  {
    /* We are in cell reselection */
    return(TRUE);
  }
  else
  {
    /* Not in Cell Reselection at present */
    return(FALSE);
  }
}

/*===========================================================================

FUNCTION          RRC_CCM_PROPAGATE_NEW_CELL_TO_ALL_PROCS

DESCRIPTION       This function is responsible for propagating the received 
                  TDSRRC_NEW_CELL_IND to all procedures that deserve to be 
                  informed of it. 
                  If a procedure's ID appears as "No Ind Procedure ID" in the
                  cmd, it is NOT informed of TDSRRC_NEW_CELL_IND.
                  
DEPENDENCIES
                  None

RETURN VALUE
                  None
SIDE EFFECTS
                  A possible side effect could come from the order in which
                  procedures are informed. The Cell Selection Procedure must
                  always be informed (if not indicated otherwise in cmd) before
                  any other procedures.
===========================================================================*/
void  tdsrrcccm_propagate_new_cell_to_all_procs( tdsrrc_cmd_type  *cmd_ptr )
{
  /* First look at the New Cell status. Different set of procedures need be
   * informed depending on if a New Cell has been selected or if there has
   * been a Cell Loss without new cell selection.
   */
  tdsrrc_state_e_type rrc_state;
  rrc_state = tdsrrc_get_state();

  if(cmd_ptr->cmd.new_cell_ind.new_cell_status == TDSRRC_NEW_CELL_SELECTED)
  {
    /* New Cell selected */
    /* Send New Cell Ind to the relevant procedures. The procedure corresponding
     * to "No Ind Procedure ID" in the cmd must not get the Ind.
     */

    /* CELL SELECTION Procedure - should always be the first one to be in synch
     * with any changes related to Cell.
     */
    if((cmd_ptr->cmd.new_cell_ind.no_ind_proc_id != TDSRRC_PROCEDURE_CSP) &&
       (cmd_ptr->cmd.new_cell_ind.cell_same_as_before == FALSE))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"New Cell Ind (New Cell) sent to Proc %d",TDSRRC_PROCEDURE_CSP);
      tdsrrccsp_cell_selection_procedure(cmd_ptr);
    }

    /* Lower Layer Controller Procedure */
    if((rrc_state == TDSRRC_STATE_CELL_FACH) || (rrc_state == TDSRRC_STATE_CELL_PCH) ||
	    (rrc_state == TDSRRC_STATE_URA_PCH) || (rrc_state == TDSRRC_STATE_CONNECTING) || 
        (rrc_state == TDSRRC_STATE_DISCONNECTED))
    {
      if((cmd_ptr->cmd.new_cell_ind.no_ind_proc_id != TDSRRC_PROCEDURE_LLC) &&
         (cmd_ptr->cmd.new_cell_ind.cell_same_as_before == FALSE))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"New Cell Ind (New Cell) sent to Proc %d",TDSRRC_PROCEDURE_LLC);
        tdsrrcllc_process_cmd(cmd_ptr);
      }
    }

    /* RRC CONNECTION ESTABLISHMENT Procedure should be sent the New Cell Ind
    * only if it last checked the camping status and found it to be "Not Camping"
    */

    if(rrc_state == TDSRRC_STATE_DISCONNECTED)
    {
      if((cmd_ptr->cmd.new_cell_ind.no_ind_proc_id != TDSRRC_PROCEDURE_RCE) &&
         (cmd_ptr->cmd.new_cell_ind.inform_procedure_rce))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"New Cell Ind (New Cell) sent to Proc %d",TDSRRC_PROCEDURE_RCE);
        /* Call RRC Connection Establishment procedure here */
        tdsrrcrce_procedure_event_handler(cmd_ptr);
      }
      else if(tdsrrcrce_get_rce_sub_state()  ==  TDSRRCRCE_WAIT_FOR_SIB7_CURRENT)
      {
        /*RCE procedure needs New Cell Ind  in its sub state wait for sib7 to get current.
          Since cell reselection should be considered as implicate SIB7 update */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"New Cell Ind (New Cell) sent to Proc %d",TDSRRC_PROCEDURE_RCE);
        /* Call RRC Connection Establishment procedure here */
        tdsrrcrce_procedure_event_handler(cmd_ptr);
      }
    }
    
    /* Send the NEW_CELL_IND to Cell Update Procedure only if the current RRC state is
     * not DISCONNECTED
     */
    if((rrc_state == TDSRRC_STATE_CELL_FACH) || (rrc_state == TDSRRC_STATE_CELL_PCH) ||
	   (rrc_state == TDSRRC_STATE_URA_PCH))
    {
      if(cmd_ptr->cmd.new_cell_ind.no_ind_proc_id != TDSRRC_PROCEDURE_CU) 
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"New Cell Ind (New Cell) sent to Proc %d",TDSRRC_PROCEDURE_CU);
        tdsrrccu_procedure_event_handler(cmd_ptr);
      }
    }
    else if(rrc_state == TDSRRC_STATE_CONNECTING)
    {
      if(cmd_ptr->cmd.new_cell_ind.no_ind_proc_id != TDSRRC_PROCEDURE_RCE)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"New Cell Ind (New Cell) sent to Proc %d",TDSRRC_PROCEDURE_RCE);
        tdsrrcrce_procedure_event_handler(cmd_ptr);
      }
    }
    
    /* Add new procedures - determine their relative priority and put them in 
     * the corect order.
     */

  } /* New cell selected */

  else if(cmd_ptr->cmd.new_cell_ind.new_cell_status == TDSRRC_CELL_LOSS_NO_NEW_CELL)
  {
    /* Cell loss with no new cell */
    /* CELL SELECTION Procedure - should always be the first one to be in synch
     * with any changes related to Cell.
     */
    if(cmd_ptr->cmd.new_cell_ind.no_ind_proc_id != TDSRRC_PROCEDURE_CSP)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"New Cell Ind (Cell Loss) sent to Proc %d",TDSRRC_PROCEDURE_CSP);
      tdsrrccsp_cell_selection_procedure(cmd_ptr);
    }

  } /* Cell loss with no new cell */

  else /* Invalid Cell Status type */
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Cell Status %d in New Cell Ind",cmd_ptr->cmd.new_cell_ind.new_cell_status);
  }

} /* tdsrrcccm_propagate_new_cell_to_all_procs  */



/*===========================================================================

FUNCTION          RRC_CCM_SEND_CHAN_CONFIG_CMD

DESCRIPTION       This function is responsible for sending 
                  TDSRRC_CHANNEL_CONFIG_REQ. This function decides
                  on the action needed on RB0 (CCCH) and State Change
                  based on the passed current RRC state.
                  It also sends an Establishment request for PCCH. 
                  NOTE: Cell Change Manager only limits to RB0 handling in
                  specific scenarios. 
                  TBD: If there is a need to reconfig PCCH on a cell change, 
                  how is that handled and who handles it ?
                  RB Setups/Releases for RB0 and other RBs are handled 
                  by other procedures.

DEPENDENCIES
                  None.

RETURN VALUE
                  None.

SIDE EFFECTS      None.
===========================================================================*/
static void  tdsrrcccm_send_chan_config_cmd
(
 tdsrrc_state_e_type  current_rrc_state,
 tdsrrc_cell_change_e_type  cell_change_type
)
{

  tdsrrc_cmd_type   *csp_out_cmd_ptr;
  uint32 count=0;
  uint32 urb_count=0;
  uint32 rab_count;
  uint32 rb_count;
  rlc_lc_id_type ul_rlc_lc_id = TDSRRCLCM_RLC_LC_ID_NOT_FOUND;
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsrrc_RB_Identity rb_id;
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

  if((csp_out_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    csp_out_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;
    csp_out_cmd_ptr->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_CCM;
    csp_out_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = FALSE;
    csp_out_cmd_ptr->cmd.chan_config_req.rrc_channel_config_cnf_required = TRUE;
    csp_out_cmd_ptr->cmd.chan_config_req.chan_config_reason = TDSRRCLLC_CHAN_CFG_REASON_NONE;

    switch(current_rrc_state)
    {
      case TDSRRC_STATE_CONNECTING:

        if(cell_change_type == TDSRRC_CHANGE_RETURN_TO_SERVICE_AREA)
        {
          /* Setup CCCH, drop BCCH */
          csp_out_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = FALSE;
          csp_out_cmd_ptr->cmd.chan_config_req.num_rb_to_config = 2;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_id = CCCH_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_config = TDSESTABLISH_RB;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[1].rb_id = BCCH_S_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[1].rb_config = TDSRELEASE_RB;
          csp_out_cmd_ptr->cmd.chan_config_req.chan_config_reason = 
            TDSRRCLLC_CHAN_CFG_REASON_RE_ENTERING_SERVICE;
        }
        else
        {
          ERR_FATAL("Cell chg type %d invalid in RRC st %d",cell_change_type,current_rrc_state,0); 
        }
        break;

      case TDSRRC_STATE_CELL_FACH:
        if(cell_change_type == TDSRRC_CHANGE_RETURN_TO_SERVICE_AREA)
        {
          csp_out_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = FALSE;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count].rb_id = CCCH_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = TDSESTABLISH_RB;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count].rb_id = BCCH_S_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = TDSRELEASE_RB;

          /*add dcch+dtch RBs in to chan config. Read this info from
          established_rabs variable */
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count].rb_id = DCCH_UM_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = TDSRECONFIG_RB;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count].rb_id = DCCH_AM_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = TDSRECONFIG_RB;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count].rb_id = DCCH_DT_HIGH_PRI_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = TDSRECONFIG_RB;
          /* check whether RB4 was present or not */
          ul_rlc_lc_id = tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH, 
                                       DCCH_DT_LOW_PRI_RADIO_BEARER_ID,
                                       UE_MODE_ACKNOWLEDGED_DATA);

          if(ul_rlc_lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"No RB4 in CELL_FACH");
          }
          else 
          {
            csp_out_cmd_ptr->cmd.chan_config_req.rb[count].rb_id = DCCH_DT_LOW_PRI_RADIO_BEARER_ID;
            csp_out_cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = TDSRECONFIG_RB;
          }
          /*setup the user plane RBs also at this time. Do not have to worry about the
          ul transmission as it will not happen until we get a valid C-RNTI in the 
          cell update confirm message */
          for (rab_count=0; rab_count<MAX_RAB_TO_SETUP; rab_count++)
          {
            if (
                 ((tdsrrc_est_rabs.rabs[rab_count].cn_domain == RRC_CS_DOMAIN_CN_ID)|| 
                   (tdsrrc_est_rabs.rabs[rab_count].cn_domain == RRC_PS_DOMAIN_CN_ID))
                  &&
                  (tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab > 0)
                )
            {
              /*it is a valid established rab, now compare each rb-id in this established rab with 
              the rb-id received in the message */
              for (rb_count=0; ((rb_count<tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab) && (rb_count<MAX_RB_PER_RAB)&&(count < TDS_MAX_RB)); rb_count++)
              {
                csp_out_cmd_ptr->cmd.chan_config_req.rb[count].rb_id = 
                  tdsrrc_est_rabs.rabs[rab_count].rb_for_rab[rb_count].rb_id;
                csp_out_cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = TDSRECONFIG_RB;
                urb_count++;
              }
            } /*end of if*/
          }/* rab_count loop */

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
          if((tdsrrclcm_get_rb_id_if_dl_lc_type_exists(UE_LOGCHAN_TM_DCCH_DL, &rb_id) == TDSRRCLCM_SUCCESS)
            &&(count<TDS_MAX_RB))
          {
            csp_out_cmd_ptr->cmd.chan_config_req.rb[count].rb_id = rb_id;
            csp_out_cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = TDSRECONFIG_RB;
          }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

          csp_out_cmd_ptr->cmd.chan_config_req.num_rb_to_config = (uint16)count;

          csp_out_cmd_ptr->cmd.chan_config_req.release_phychls = FALSE;
          csp_out_cmd_ptr->cmd.chan_config_req.release_trchls = FALSE;
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"TDSRRC_CHANNEL_CONFIG_REQ to reconfig %dRBs,uplane RBs:%d",count,urb_count);
          csp_out_cmd_ptr->cmd.chan_config_req.chan_config_reason = 
            TDSRRCLLC_CHAN_CFG_REASON_RE_ENTERING_SERVICE;
        }
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
        else if(cell_change_type == TDSRRC_CHANGE_CELL_FACH_TO_CELL_FACH)
        {
          /* Add PCCH, drop BCCH */
          csp_out_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = FALSE;
          csp_out_cmd_ptr->cmd.chan_config_req.next_state = TDSRRC_STATE_CELL_FACH;
          
        

          csp_out_cmd_ptr->cmd.chan_config_req.rb[count].rb_id = CCCH_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = TDSRECONFIG_RB;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count].rb_id = BCCH_S_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = TDSRELEASE_RB;

           /*add dcch+dtch RBs in to chan config. Read this info from
          established_rabs variable */
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count].rb_id = DCCH_UM_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = TDSRECONFIG_RB;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count].rb_id = DCCH_AM_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = TDSRECONFIG_RB;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count].rb_id = DCCH_DT_HIGH_PRI_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = TDSRECONFIG_RB;
          /* check whether RB4 was present or not */
          ul_rlc_lc_id = tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH, 
                                       DCCH_DT_LOW_PRI_RADIO_BEARER_ID,
                                       UE_MODE_ACKNOWLEDGED_DATA);

          if(ul_rlc_lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"No RB4 in CELL_FACH");
          }
          else 
          {
            csp_out_cmd_ptr->cmd.chan_config_req.rb[count].rb_id = DCCH_DT_LOW_PRI_RADIO_BEARER_ID;
            csp_out_cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = TDSRECONFIG_RB;
          }
          /*setup the user plane RBs also at this time. Do not have to worry about the
          ul transmission as it will not happen until we get a valid C-RNTI in the 
          cell update confirm message */
          for (rab_count=0; rab_count<MAX_RAB_TO_SETUP; rab_count++)
          {
            if (
                 ((tdsrrc_est_rabs.rabs[rab_count].cn_domain == RRC_CS_DOMAIN_CN_ID)|| 
                   (tdsrrc_est_rabs.rabs[rab_count].cn_domain == RRC_PS_DOMAIN_CN_ID))
                  &&
                  (tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab > 0)
                )
            {
              /*it is a valid established rab, now compare each rb-id in this established rab with 
              the rb-id received in the message */
              for (rb_count=0; ((rb_count<tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab) && (rb_count<MAX_RB_PER_RAB)&&(count < TDS_MAX_RB)); rb_count++)
              {
                csp_out_cmd_ptr->cmd.chan_config_req.rb[count].rb_id = 
                  tdsrrc_est_rabs.rabs[rab_count].rb_for_rab[rb_count].rb_id;
                if(count < (TDS_MAX_RB -1))
                {
                    csp_out_cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = TDSRECONFIG_RB;
                }
                urb_count++;
              }
            } /*end of if*/
          }/* rab_count loop */

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
          if((tdsrrclcm_get_rb_id_if_dl_lc_type_exists(UE_LOGCHAN_TM_DCCH_DL, &rb_id) == TDSRRCLCM_SUCCESS)
            &&(count < TDS_MAX_RB))
          {
            csp_out_cmd_ptr->cmd.chan_config_req.rb[count].rb_id = rb_id;
            if(count < (TDS_MAX_RB -1))
            {
                csp_out_cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = TDSRECONFIG_RB;
            }
          }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
          csp_out_cmd_ptr->cmd.chan_config_req.num_rb_to_config = count;
          csp_out_cmd_ptr->cmd.chan_config_req.release_phychls = FALSE;
          csp_out_cmd_ptr->cmd.chan_config_req.release_trchls = FALSE;
          if((tdsrrcllc_get_toc_usage() == TDSTOC_FOR_OOS_WITHOUT_DCH_INFO) || 
             (tdsrrcllc_get_toc_usage() == TDSTOC_FOR_OOS_WITH_DCH_INFO))
          {
            csp_out_cmd_ptr->cmd.chan_config_req.chan_config_reason = 
                                                                      TDSRRCLLC_CHAN_CFG_REASON_RE_ENTERING_SERVICE;
          }
          else
          {
            csp_out_cmd_ptr->cmd.chan_config_req.chan_config_reason = 
                                                        TDSRRCLLC_CHAN_CFG_REASON_WITHIN_FACH;
          }
        }
        else if(cell_change_type == TDSRRC_CHANGE_CELL_FACH_TO_CELL_PCH)
        {
          /* Add PCCH, drop BCCH */
          csp_out_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = TRUE;
          csp_out_cmd_ptr->cmd.chan_config_req.next_state = TDSRRC_STATE_CELL_PCH;
          
          csp_out_cmd_ptr->cmd.chan_config_req.num_rb_to_config = 2;

          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_id = PCCH_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_config = TDSESTABLISH_RB;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[1].rb_id = BCCH_S_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[1].rb_config = TDSRELEASE_RB;

          csp_out_cmd_ptr->cmd.chan_config_req.chan_config_reason = 
                                                                       TDSRRCLLC_CHAN_CFG_REASON_FACH_TO_PCH;
          csp_out_cmd_ptr->cmd.chan_config_req.release_phychls = FALSE;
          csp_out_cmd_ptr->cmd.chan_config_req.release_trchls = FALSE;
        }
        else if(cell_change_type == TDSRRC_CHANGE_CELL_FACH_TO_URA_PCH)
        {
          /* Add PCCH, drop BCCH */
          csp_out_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = TRUE;
          csp_out_cmd_ptr->cmd.chan_config_req.next_state = TDSRRC_STATE_URA_PCH;
          
          csp_out_cmd_ptr->cmd.chan_config_req.num_rb_to_config = 2;

          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_id = PCCH_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_config = TDSESTABLISH_RB;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[1].rb_id = BCCH_S_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[1].rb_config = TDSRELEASE_RB;

          csp_out_cmd_ptr->cmd.chan_config_req.chan_config_reason = 
                                                                       TDSRRCLLC_CHAN_CFG_REASON_FACH_TO_PCH;
          csp_out_cmd_ptr->cmd.chan_config_req.release_phychls = FALSE;
          csp_out_cmd_ptr->cmd.chan_config_req.release_trchls = FALSE;
        }
#endif
        else
        {
          csp_out_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = FALSE;
          csp_out_cmd_ptr->cmd.chan_config_req.num_rb_to_config = 1;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_id = CCCH_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_config = TDSRECONFIG_RB;
        }
        break;

      case TDSRRC_STATE_CELL_PCH:
        if(cell_change_type == TDSRRC_CHANGE_RETURN_TO_SERVICE_AREA)
        {
          csp_out_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = FALSE;
          csp_out_cmd_ptr->cmd.chan_config_req.next_state = TDSRRC_STATE_CELL_PCH;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count].rb_id = PCCH_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = TDSESTABLISH_RB;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count].rb_id = BCCH_S_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[count++].rb_config = TDSRELEASE_RB;
          csp_out_cmd_ptr->cmd.chan_config_req.num_rb_to_config = (uint16)count;

          csp_out_cmd_ptr->cmd.chan_config_req.release_phychls = FALSE;
          csp_out_cmd_ptr->cmd.chan_config_req.release_trchls = FALSE;
          csp_out_cmd_ptr->cmd.chan_config_req.chan_config_reason = 
            TDSRRCLLC_CHAN_CFG_REASON_RE_ENTERING_SERVICE;

        }
        else
        {
          csp_out_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = TRUE;
          csp_out_cmd_ptr->cmd.chan_config_req.next_state = TDSRRC_STATE_CELL_FACH;
          csp_out_cmd_ptr->cmd.chan_config_req.num_rb_to_config = 1;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_id = CCCH_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_config = TDSESTABLISH_RB;
        }
        break;

      case TDSRRC_STATE_URA_PCH:
        if(cell_change_type == TDSRRC_CHANGE_RETURN_TO_SERVICE_AREA)
        {
          csp_out_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = FALSE;
          csp_out_cmd_ptr->cmd.chan_config_req.next_state = TDSRRC_STATE_URA_PCH;
          csp_out_cmd_ptr->cmd.chan_config_req.num_rb_to_config = 2;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_id = PCCH_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_config = TDSESTABLISH_RB;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[1].rb_id = BCCH_S_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[1].rb_config = TDSRELEASE_RB;
          csp_out_cmd_ptr->cmd.chan_config_req.release_phychls = FALSE;
          csp_out_cmd_ptr->cmd.chan_config_req.release_trchls = FALSE;
          csp_out_cmd_ptr->cmd.chan_config_req.chan_config_reason = 
            TDSRRCLLC_CHAN_CFG_REASON_RE_ENTERING_SERVICE;
        }
        else
        {
          csp_out_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = TRUE;
          csp_out_cmd_ptr->cmd.chan_config_req.next_state = TDSRRC_STATE_CELL_FACH;
          csp_out_cmd_ptr->cmd.chan_config_req.num_rb_to_config = 1;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_id = CCCH_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_config = TDSESTABLISH_RB;
        }
        break;

      case TDSRRC_STATE_DISCONNECTED:
        if(cell_change_type == TDSRRC_CHANGE_CELL_SELECTION)
        {
          /* Add PCCH, drop BCCH */
          csp_out_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = FALSE;
          csp_out_cmd_ptr->cmd.chan_config_req.num_rb_to_config = 2;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_id = PCCH_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_config = TDSESTABLISH_RB;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[1].rb_id = BCCH_S_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[1].rb_config = TDSRELEASE_RB;
        }
        else
        {
          ERR_FATAL("Cell Chg type %d invalid in RRC st %d",cell_change_type,current_rrc_state,0); 
        }
        break;

      case TDSRRC_STATE_CELL_DCH:
        if(cell_change_type == TDSRRC_CHANGE_CELL_DCH_TO_CELL_FACH)
        {
          /* Add PCCH, drop BCCH */
          csp_out_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = TRUE;
          csp_out_cmd_ptr->cmd.chan_config_req.next_state = TDSRRC_STATE_CELL_FACH;
          
          csp_out_cmd_ptr->cmd.chan_config_req.num_rb_to_config = 2;

          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_id = CCCH_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_config = TDSESTABLISH_RB;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[1].rb_id = BCCH_S_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[1].rb_config = TDSRELEASE_RB;

          csp_out_cmd_ptr->cmd.chan_config_req.release_phychls = FALSE;
          csp_out_cmd_ptr->cmd.chan_config_req.release_trchls = FALSE;
        }
        else if(cell_change_type == TDSRRC_CHANGE_CELL_DCH_TO_CELL_PCH)
        {
          /* Add PCCH, drop BCCH */
          csp_out_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = TRUE;
          csp_out_cmd_ptr->cmd.chan_config_req.next_state = TDSRRC_STATE_CELL_PCH;
          
          csp_out_cmd_ptr->cmd.chan_config_req.num_rb_to_config = 2;

          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_id = PCCH_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_config = TDSESTABLISH_RB;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[1].rb_id = BCCH_S_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[1].rb_config = TDSRELEASE_RB;

          csp_out_cmd_ptr->cmd.chan_config_req.release_phychls = FALSE;
          csp_out_cmd_ptr->cmd.chan_config_req.release_trchls = FALSE;
        }
        else if(cell_change_type == TDSRRC_CHANGE_CELL_DCH_TO_URA_PCH)
        {
          /* Add PCCH, drop BCCH */
          csp_out_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = TRUE;
          csp_out_cmd_ptr->cmd.chan_config_req.next_state = TDSRRC_STATE_URA_PCH;
          
          csp_out_cmd_ptr->cmd.chan_config_req.num_rb_to_config = 2;

          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_id = PCCH_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_config = TDSESTABLISH_RB;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[1].rb_id = BCCH_S_RADIO_BEARER_ID;
          csp_out_cmd_ptr->cmd.chan_config_req.rb[1].rb_config = TDSRELEASE_RB;

          csp_out_cmd_ptr->cmd.chan_config_req.release_phychls = FALSE;
          csp_out_cmd_ptr->cmd.chan_config_req.release_trchls = FALSE;
        }
        else
        {
          ERR_FATAL("Cell Chg type %d rxed in CELL_DCH",cell_change_type,0,0);
        }
        break;

      default:
        ERR_FATAL("No Channel Config to LLC in RRC State %d",current_rrc_state,0,0);
    }

    /* Send the Internal RRC Command - LLC should directly handle it */
    tdsrrc_put_int_cmd( csp_out_cmd_ptr );
  }
  else /* No cmd buffer for RRC Internal command */
  {
    ERR_FATAL("Unable to get Int Cmd buf for RRC Cmd %d",TDSRRC_CHANNEL_CONFIG_REQ,0,0);
  }


} /* tdsrrcccm_send_chan_config_cmd */


/*===========================================================================

FUNCTION          tdsrrcccm_send_mm_service_ind

DESCRIPTION       This function is responsible for sending RRC_SERVICE_IND
                  to MM.

DEPENDENCIES
                  SIB1 should be available.

RETURN VALUE
                  SUCCESS: Able to send RRC_SERVICE_IND successfully.
                  FAILURE: Not able to send due to any of the multiple reasons.

SIDE EFFECTS      None.
===========================================================================*/
static uecomdef_status_e_type tdsrrcccm_send_mm_service_ind(void)
{
  /* TBD: Send the NAS system info from SIB1 to MM */

  return(SUCCESS);
}

/*===========================================================================

FUNCTION          tdsrrcccm_fill_nas_cn_specific_info

DESCRIPTION       This function is responsible for filling the Core Network 
                  information to MM.

DEPENDENCIES
                  

RETURN VALUE
                  SUCCESS: Able to fill the CN info successfully.
                  FAILURE: Not able to fill the CN info.

SIDE EFFECTS      None.
===========================================================================*/
static uecomdef_status_e_type  
tdsrrcccm_fill_nas_cn_specific_info
(
  tdsrrc_CN_DomainInformation rrc_cn_domain_information,
  rrc_cn_domain_sys_info_type  *rrc_cn_domain_sys_info_ptr
)
{
  unsigned char  *tdsdata_ptr;
  uint32          i;

  if(rrc_cn_domain_information.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain)
  {
    rrc_cn_domain_sys_info_ptr->cn_domain_id = RRC_CS_DOMAIN_CN_ID; 
  }
  else
  {
    rrc_cn_domain_sys_info_ptr->cn_domain_id = RRC_PS_DOMAIN_CN_ID;
  }
    

  /* Check if the RRC MM interface structure array can hold this info */
  if( TDSRRC_GET_OCTET_STRING_LEN(rrc_cn_domain_information.cn_DomainSpecificNAS_Info) >
     RRC_MAX_GSM_INFO_OCTET_COUNT)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Not enough space for NAS Sys Info (%d bytes)",TDSRRC_GET_OCTET_STRING_LEN(rrc_cn_domain_information.cn_DomainSpecificNAS_Info));
    return(FAILURE);
  }
  
  rrc_cn_domain_sys_info_ptr->cn_domain_specific_info.gsm_map_info.length 
  = TDSRRC_GET_OCTET_STRING_LEN(rrc_cn_domain_information.cn_DomainSpecificNAS_Info);

  tdsdata_ptr = TDSRRC_GET_OCTET_STRING_PTR(rrc_cn_domain_information.cn_DomainSpecificNAS_Info);

  for( i = 0; i < TDSRRC_GET_OCTET_STRING_LEN(rrc_cn_domain_information.cn_DomainSpecificNAS_Info); i ++)
  {
    rrc_cn_domain_sys_info_ptr->cn_domain_specific_info.gsm_map_info.value[i] =
     *tdsdata_ptr;

    tdsdata_ptr ++;
  }

  return(SUCCESS);
} /* tdsrrcccm_fill_nas_cn_specific_info */
/*===========================================================================

FUNCTION          tdsrrcccm_fill_nas_cn_specific_info_full

DESCRIPTION       This function is responsible for filling the Core Network 
                  information to MM.

DEPENDENCIES
                  

RETURN VALUE
                  SUCCESS: Able to fill the CN info successfully.
                  FAILURE: Not able to fill the CN info.

SIDE EFFECTS      None.
===========================================================================*/
static uecomdef_status_e_type  
tdsrrcccm_fill_nas_cn_specific_info_full
(
  tdsrrc_CN_DomainInformationFull rrc_cn_domain_information_full,
  rrc_cn_domain_sys_info_type  *rrc_cn_domain_sys_info_ptr
)
{
  unsigned char  *tdsdata_ptr;
  uint32          i;

  if(rrc_cn_domain_information_full.cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain)
  {
    rrc_cn_domain_sys_info_ptr->cn_domain_id = RRC_CS_DOMAIN_CN_ID; 
  }
  else
  {
    rrc_cn_domain_sys_info_ptr->cn_domain_id = RRC_PS_DOMAIN_CN_ID;
  }
  /* Check if the RRC MM interface structure array can hold this info */
  if((TDSRRC_GET_OCTET_STRING_LEN(rrc_cn_domain_information_full.cn_DomainSpecificNAS_Info) >
     RRC_MAX_GSM_INFO_OCTET_COUNT))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Not enough space for NAS Sys Info (%d bytes)",TDSRRC_GET_OCTET_STRING_LEN(rrc_cn_domain_information_full.cn_DomainSpecificNAS_Info));
    return(FAILURE);
  }
  
  rrc_cn_domain_sys_info_ptr->cn_domain_specific_info.gsm_map_info.length 
  = TDSRRC_GET_OCTET_STRING_LEN(rrc_cn_domain_information_full.cn_DomainSpecificNAS_Info);

  tdsdata_ptr = TDSRRC_GET_OCTET_STRING_PTR(rrc_cn_domain_information_full.cn_DomainSpecificNAS_Info);

  for( i = 0; i < TDSRRC_GET_OCTET_STRING_LEN(rrc_cn_domain_information_full.cn_DomainSpecificNAS_Info); i ++)
  {
    rrc_cn_domain_sys_info_ptr->cn_domain_specific_info.gsm_map_info.value[i] =
     *tdsdata_ptr;

    tdsdata_ptr ++;
  }

  return(SUCCESS);
} /* tdsrrcccm_fill_nas_cn_specific_info_full */


/*===========================================================================

FUNCTION          tdsrrcccm_buid_service_ind_with_nas_cn_info_r6

DESCRIPTION       This function is responsible for building service_ind with
                  the NAS specific CN information.

DEPENDENCIES
                  

RETURN VALUE
                  mm_cmd_type * cmd: mm command

SIDE EFFECTS      None.
===========================================================================*/
mm_cmd_type *
tdsrrcccm_buid_service_ind_with_nas_cn_info_r6
(
  tdsrrc_CN_InformationInfo_r6* cn_information_info_ptr
)
{
  /* Send the CN info to MM */

  mm_cmd_type *mm_buf_ptr = NULL;
  uint32 num_cn_domains = 0;
  unsigned char  *tdsdata_ptr;
  uint32          i;
  tdsrrc_int_cell_id_type cell_id = 0;
  rrc_plmn_identity_type plmn_id;
 
 

  if(cn_information_info_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"NULL Pointer for CN Information");
    return(mm_buf_ptr);
  }

  /* Get a buffer from MM task */
  if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_SERVICE_IND_FROM_OTA_MSG)) == NULL)
  {
    ERR_FATAL("No MM buffer for CSP to MM cmd 0x%x",RRC_SERVICE_IND_FROM_OTA_MSG,0,0);
  }

#ifdef FEATURE_DUAL_SIM
  mm_buf_ptr->cmd.rrc_service_ind.as_id = tdsrrc_get_as_id();
#endif

  /* Fill in the service status */
  if ((tdsrrcccm_get_serv_plmn_cellid(&cell_id, &plmn_id) == TDSRRC_CELL_LOSS_NO_NEW_CELL))
  {
    mm_buf_ptr->cmd.rrc_service_ind.service_status = MM_AS_NO_SERVICE;
  }
  else
  {
    mm_buf_ptr->cmd.rrc_service_ind.service_status = MM_AS_SERVICE_AVAILABLE;
  }

  mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.is_cell_id_valid = FALSE;

  if(mm_buf_ptr->cmd.rrc_service_ind.service_status == MM_AS_SERVICE_AVAILABLE)
  {

    if (tdsrrc_ccm_cell_camping_info.is_curr_cell_id_valid)
    {
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.is_cell_id_valid = TRUE;
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.cell_id = cell_id;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Reporting Cell_ID :%d to NAS",cell_id);
    }

    /* Fill the cell access */
    mm_buf_ptr->cmd.rrc_service_ind.service_info.cell_access =
      tdsrrc_csp_int_data.curr_cell_access_status;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Cell Access Info when ccm build service Ind with nas CN info= %d ",mm_buf_ptr->cmd.rrc_service_ind.service_info.cell_access);	
#ifdef FEATURE_DSAC
    if(tdsrrc_csp_int_data.is_dsac_valid == TRUE)
    {
       mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present  = 
                                                 tdsrrc_csp_int_data.is_dsac_valid;
       mm_buf_ptr->cmd.rrc_service_ind.service_info.cs_cell_access = 
                                                 tdsrrc_csp_int_data.curr_cs_cell_access;
       mm_buf_ptr->cmd.rrc_service_ind.service_info.ps_cell_access = 
                                                 tdsrrc_csp_int_data.curr_ps_cell_access;
       TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: DSAC, CS and PS cell access values are %d,  %d, %d", 
                        mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present,
                        mm_buf_ptr->cmd.rrc_service_ind.service_info.cs_cell_access,
                        mm_buf_ptr->cmd.rrc_service_ind.service_info.ps_cell_access);
    }
    else
    {
      mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present = tdsrrc_csp_int_data.is_dsac_valid;
      mm_buf_ptr->cmd.rrc_service_ind.service_info.cell_access = tdsrrc_csp_int_data.curr_cell_access_status;
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"DSAC: DSAC and cell access values are %d,%d",mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present,mm_buf_ptr->cmd.rrc_service_ind.service_info.cell_access);
    }

    if(tdsrrc_csp_int_data.is_dsac_valid == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC info is valid for CCM");
      tdsrrc_ccm_cell_camping_info.curr_dsac_present = tdsrrc_csp_int_data.is_dsac_valid;
      tdsrrc_ccm_cell_camping_info.curr_cs_cell_access =   
                                            mm_buf_ptr->cmd.rrc_service_ind.service_info.cs_cell_access;
      tdsrrc_ccm_cell_camping_info.curr_ps_cell_access =   
                                 mm_buf_ptr->cmd.rrc_service_ind.service_info.ps_cell_access;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: DSAC, CS and PS access values in ccm cell camp info %d,  %d, %d", 
                                tdsrrc_ccm_cell_camping_info.curr_dsac_present,
                                tdsrrc_ccm_cell_camping_info.curr_cs_cell_access,
                                tdsrrc_ccm_cell_camping_info.curr_ps_cell_access);
    }
    else
    {
       TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC info is not valid for CCM");
       tdsrrc_ccm_cell_camping_info.curr_dsac_present = tdsrrc_csp_int_data.is_dsac_valid;
    }
#endif

    /* Fill the PLMN information */
    mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_type = RRC_GSM_MAP_PLMN_SEL;

    mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info_incl = FALSE;

    /*lint -e737*/
    /*lint -e650*/

    if(/*Check to see if Primary PLMN I.D is present */
       (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cn_information_info_ptr,
       tdsrrc_CN_InformationInfo_r6,primary_plmn_Identity))
       ||/*If not then check to see if common PLMN I.D is present */
       (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cn_information_info_ptr,
       tdsrrc_CN_InformationInfo_r6,plmn_Identity))
      )
    {
      /*lint +e737*/
      /*lint +e650*/
      /*If primary PLMN I.D is present then this should be considered 
       *  over common PLMN I.D.Since common PLMN I.D is optional field
       *  so it may not be included. In theses cases if primary PLMN I.D present
       *  honor it & send to MM
       */
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cn_information_info_ptr,
	  	tdsrrc_CN_InformationInfo_r6,primary_plmn_Identity))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS:Primary PLMN I.D present in OTA message");
        tdsrrc_translate_gsm_map_plmn_id 
         (&cn_information_info_ptr->primary_plmn_Identity,
          &mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_id
         );
        tdsrrc_ccm_cell_camping_info.curr_cell.plmn_id = 
         mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_id;
      }
      else
      {
        tdsrrc_translate_gsm_map_plmn_id 
         (&cn_information_info_ptr->plmn_Identity,
          &mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_id
         );
      }
      /* Update the current requested PLMN with the new PLMN */
      tdsrrc_csp_int_data.curr_req_plmn.plmn_id  = 
        mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_id;

      tdsrrc_print_plmn_id(tdsrrc_csp_int_data.curr_req_plmn.plmn_id);
    }
    else
    {
            mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[0]= 0xFF;
            mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[1]= 0xFF;
            mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[2]= 0xFF;
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: Set mcc[0]=0x%x, mcc[1]=0x%x, mcc[2]=0x%x in RRC_SERVICE_IND_FROM_OTA_MSG ",
                                                 mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[0],
                                                 mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[1],
                                                 mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[2]);
    }

    /* Fill in the CN common GSM-MAP NAS system information now */                                                
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cn_information_info_ptr,
		tdsrrc_CN_InformationInfo_r6,cn_CommonGSM_MAP_NAS_SysInfo))
    {
      mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info_incl = TRUE;
      mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info.length =
      TDSRRC_GET_OCTET_STRING_LEN(cn_information_info_ptr->cn_CommonGSM_MAP_NAS_SysInfo);

      tdsdata_ptr = TDSRRC_GET_OCTET_STRING_PTR(cn_information_info_ptr->cn_CommonGSM_MAP_NAS_SysInfo);

      for( i = 0; i < TDSRRC_GET_OCTET_STRING_LEN(cn_information_info_ptr->cn_CommonGSM_MAP_NAS_SysInfo); i ++)
      {
        mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info.value[i] =
         *tdsdata_ptr;

        tdsdata_ptr ++;
      }
      /*Store the LAC from the OTA in global variable tdslac_in_ota*/	  
      if(tdsrrccsp_get_lac_id_in_dch(&(mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info)) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not able to get LAC from CN Info.");
      }	  
    }

    /* Fill in the CN domain specific NAS system info now */

    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cn_information_info_ptr,
		tdsrrc_CN_InformationInfo_r6,cn_DomainInformationList))
    {
      uint32 idx =0;
      /* Save the head pointer of the linked list */
     

      do
      {
        if(tdsrrcccm_fill_nas_cn_specific_info(cn_information_info_ptr->cn_DomainInformationList.elem[idx],
            &mm_buf_ptr->cmd.rrc_service_ind.service_info.
             cn_domain_info[num_cn_domains]) == FAILURE)
        {
          mm_free_cmd_buf(mm_buf_ptr);
          return((mm_cmd_type *)NULL);
        }
        num_cn_domains ++;
        idx++;

      }while((cn_information_info_ptr->cn_DomainInformationList.n >idx) &&
            (num_cn_domains < RRC_MAX_CN_DOMAINS));

#ifdef FEATURE_TDSCDMA_MODEM_STATISTICS
      tdsrrccsp_get_rac_id_in_dch(&(cn_information_info_ptr->cn_DomainInformationList));
#endif
     
    }

    mm_buf_ptr->cmd.rrc_service_ind.service_info.num_cn_domains =
      num_cn_domains;

  } /* Service is available */

  /* Put the command on MM queue */
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Built MM cmd 0x%x",RRC_SERVICE_IND_FROM_OTA_MSG);
  tdsrrcccm_backup_service_ind(RRC_SERVICE_IND_FROM_OTA_MSG, mm_buf_ptr);

  return(mm_buf_ptr);
} /* tdsrrcccm_buid_service_ind_with_nas_cn_info_r6 */

/*===========================================================================

FUNCTION          RRC_CCM_SEND_NAS_CN_INFO_R6

DESCRIPTION       This function is responsible for sending the NAS specific
                  CN information to MM.

DEPENDENCIES
                  

RETURN VALUE
                  SUCCESS: Able to send RRC_SERVICE_IND_FROM_OTA_MSG successfully.
                  FAILURE: Not able to send due to any of the multiple reasons.

SIDE EFFECTS      None.
===========================================================================*/
uecomdef_status_e_type tdsrrcccm_send_nas_cn_info_r6
(
  tdsrrc_CN_InformationInfo_r6* cn_information_info_ptr
)
{
  /* Send the CN info to MM */

  mm_cmd_type *mm_buf_ptr;
  uint32 num_cn_domains = 0;
  unsigned char  *tdsdata_ptr;
  uint32          i;
  tdsrrc_int_cell_id_type cell_id = 0;
  rrc_plmn_identity_type plmn_id;
 
 

  if(cn_information_info_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"NULL Pointer for CN Information");
    return(FAILURE);
  }

  /* Get a buffer from MM task */
  if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_SERVICE_IND_FROM_OTA_MSG)) == NULL)
  {
    ERR_FATAL("No MM buffer for CSP to MM cmd 0x%x",RRC_SERVICE_IND_FROM_OTA_MSG,0,0);
  }

#ifdef FEATURE_DUAL_SIM
  mm_buf_ptr->cmd.rrc_service_ind.as_id = tdsrrc_get_as_id();
#endif

  /* Fill in the service status */
  if ((tdsrrcccm_get_serv_plmn_cellid(&cell_id, &plmn_id) == TDSRRC_CELL_LOSS_NO_NEW_CELL))
  {
    mm_buf_ptr->cmd.rrc_service_ind.service_status = MM_AS_NO_SERVICE;
  }
  else
  {
    mm_buf_ptr->cmd.rrc_service_ind.service_status = MM_AS_SERVICE_AVAILABLE;
  }

  mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.is_cell_id_valid = FALSE;

  if(mm_buf_ptr->cmd.rrc_service_ind.service_status == MM_AS_SERVICE_AVAILABLE)
  {

    if (tdsrrc_ccm_cell_camping_info.is_curr_cell_id_valid)
    {
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.is_cell_id_valid = TRUE;
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.cell_id = cell_id;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Reporting Cell_ID :%d to NAS",cell_id);
    }

    /* Fill the cell access */
    mm_buf_ptr->cmd.rrc_service_ind.service_info.cell_access =
      tdsrrc_csp_int_data.curr_cell_access_status;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Cell Access Info when ccm build service Ind with nas CN info= %d ",mm_buf_ptr->cmd.rrc_service_ind.service_info.cell_access);	

#ifdef FEATURE_DSAC
    if(tdsrrc_csp_int_data.is_dsac_valid == TRUE)
    {
       TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC info is present");
       mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present  = 
                                             tdsrrc_csp_int_data.is_dsac_valid;
       mm_buf_ptr->cmd.rrc_service_ind.service_info.cs_cell_access = 
                                             tdsrrc_csp_int_data.curr_cs_cell_access;
       mm_buf_ptr->cmd.rrc_service_ind.service_info.ps_cell_access = 
                                             tdsrrc_csp_int_data.curr_ps_cell_access;
       TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: DSAC, CS and PS cell access values  are %d,  %d, %d",
                                    mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present,
                                    mm_buf_ptr->cmd.rrc_service_ind.service_info.cs_cell_access,
                                    mm_buf_ptr->cmd.rrc_service_ind.service_info.ps_cell_access);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC info is not present");
      mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present = tdsrrc_csp_int_data.is_dsac_valid;
      mm_buf_ptr->cmd.rrc_service_ind.service_info.cell_access = tdsrrc_csp_int_data.curr_cell_access_status;
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"DSAC: DSAC and cell access values  are %d,%d",mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present,mm_buf_ptr->cmd.rrc_service_ind.service_info.cell_access);
    }

    if(tdsrrc_csp_int_data.is_dsac_valid == TRUE) 
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC info is valid for CCM");
      tdsrrc_ccm_cell_camping_info.curr_dsac_present = 
                           tdsrrc_csp_int_data.is_dsac_valid;
      tdsrrc_ccm_cell_camping_info.curr_cs_cell_access =   
                           mm_buf_ptr->cmd.rrc_service_ind.service_info.cs_cell_access;
      tdsrrc_ccm_cell_camping_info.curr_ps_cell_access =   
                            mm_buf_ptr->cmd.rrc_service_ind.service_info.ps_cell_access;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: DSAC, CS and PS Access values in ccm cell camp info %d,  %d, %d", 
                            tdsrrc_ccm_cell_camping_info.curr_dsac_present,
                            tdsrrc_ccm_cell_camping_info.curr_cs_cell_access,
                            tdsrrc_ccm_cell_camping_info.curr_ps_cell_access);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC info is not valid for CCM");	  
      tdsrrc_ccm_cell_camping_info.curr_dsac_present = tdsrrc_csp_int_data.is_dsac_valid;
    }
#endif 

    /* Fill the PLMN information */
    mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_type = RRC_GSM_MAP_PLMN_SEL;

    mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info_incl = FALSE;

    /*lint -e737*/
    /*lint -e650*/
    if (/*Check to see if Primary PLMN I.D is present */
       (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cn_information_info_ptr,
       tdsrrc_CN_InformationInfo_r6,primary_plmn_Identity))
       ||/*If not then check to see if common PLMN I.D is present */
       (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cn_information_info_ptr,
       tdsrrc_CN_InformationInfo_r6,plmn_Identity))
      )
    {
      /*lint +e737*/
      /*lint +e650*/
      /*If primary PLMN I.D is present then this should be considered 
       *  over common PLMN I.D.Since common PLMN I.D is optional field
       *  so it may not be included. In theses cases if primary PLMN I.D present
       *  honor it & send to MM
       */
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cn_information_info_ptr,
       tdsrrc_CN_InformationInfo_r6,primary_plmn_Identity))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS:Primary PLMN I.D present in OTA message");
        tdsrrc_translate_gsm_map_plmn_id 
         (&cn_information_info_ptr->primary_plmn_Identity,
          &mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_id
         );
        tdsrrc_ccm_cell_camping_info.curr_cell.plmn_id = 
         mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_id;
      }
      else 
      {
        tdsrrc_translate_gsm_map_plmn_id 
         (&cn_information_info_ptr->plmn_Identity,
          &mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_id
         );
      }
      /* Update the current requested PLMN with the new PLMN */
      tdsrrc_csp_int_data.curr_req_plmn.plmn_id  = 
        mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_id;

      tdsrrc_print_plmn_id(tdsrrc_csp_int_data.curr_req_plmn.plmn_id);
    }
    else
    {
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[0]= 0xFF;
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[1]= 0xFF;
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[2]= 0xFF;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: Set mcc[0]=0x%x, mcc[1]=0x%x, mcc[2]=0x%x in RRC_SERVICE_IND_FROM_OTA_MSG ",
                                               mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[0],
                                               mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[1],
                                               mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[2]);
    }

    /* Fill in the CN common GSM-MAP NAS system information now */                                                
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cn_information_info_ptr,
		tdsrrc_CN_InformationInfo,cn_CommonGSM_MAP_NAS_SysInfo))
    {
      mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info_incl = TRUE;
      mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info.length =
      TDSRRC_GET_OCTET_STRING_LEN(cn_information_info_ptr->cn_CommonGSM_MAP_NAS_SysInfo);

      tdsdata_ptr = TDSRRC_GET_OCTET_STRING_PTR(cn_information_info_ptr->cn_CommonGSM_MAP_NAS_SysInfo);

      for( i = 0; i < TDSRRC_GET_OCTET_STRING_LEN(cn_information_info_ptr->cn_CommonGSM_MAP_NAS_SysInfo); i ++)
      {
        mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info.value[i] =
         *tdsdata_ptr;

        tdsdata_ptr ++;
      }
      /*Store the LAC from the OTA in global variable tdslac_in_ota*/	  	  
      if(tdsrrccsp_get_lac_id_in_dch(&(mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info)) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not able to get LAC from CN Info.");
      }	  
    }

    /* Fill in the CN domain specific NAS system info now */

    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cn_information_info_ptr,
		tdsrrc_CN_InformationInfo_r6,cn_DomainInformationList))
    {
     uint32 idx =0;

      do
      {
        if(tdsrrcccm_fill_nas_cn_specific_info(cn_information_info_ptr->cn_DomainInformationList.elem[idx],
            &mm_buf_ptr->cmd.rrc_service_ind.service_info.
             cn_domain_info[num_cn_domains]) == FAILURE)
        {
          mm_free_cmd_buf(mm_buf_ptr);
          return(FAILURE);
        }
        num_cn_domains ++;
        idx++;

      }while((cn_information_info_ptr->cn_DomainInformationList.n > idx) &&
            (num_cn_domains < RRC_MAX_CN_DOMAINS));
      /* Restore the head pointer of the linked list */
#ifdef FEATURE_TDSCDMA_MODEM_STATISTICS
      tdsrrccsp_get_rac_id_in_dch(&(cn_information_info_ptr->cn_DomainInformationList));
#endif    
    }

    mm_buf_ptr->cmd.rrc_service_ind.service_info.num_cn_domains =
      num_cn_domains;

  } /* Service is available */

  /* Put the command on MM queue */
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending MM cmd 0x%x",RRC_SERVICE_IND_FROM_OTA_MSG);
  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
        tdscfa_mm_put_cmd(mm_buf_ptr);
  }

  tdsrrcccm_backup_service_ind(RRC_SERVICE_IND_FROM_OTA_MSG, mm_buf_ptr);
  /* Send to external command queue */
  mm_put_cmd(mm_buf_ptr);

  return(SUCCESS);
} /* tdsrrcccm_send_nas_cn_info_r6 */



/*===========================================================================

FUNCTION          RRC_CCM_BUILD_SERVICE_IND_WITH_NAS_CN_INFO

DESCRIPTION       This function is responsible for building service_ind with
                  the NAS specific CN information.

DEPENDENCIES
                  

RETURN VALUE
                  mm_cmd_type * cmd: mm command

SIDE EFFECTS      None.
===========================================================================*/
mm_cmd_type *
tdsrrcccm_buid_service_ind_with_nas_cn_info
(
  tdsrrc_CN_InformationInfo* cn_information_info_ptr
   /*Boolean used to indicate primary plmn i.d is present in OTA msg*/
  ,boolean primary_plmn_Id_present
   /*pointer to primary plmn i.d if present in OTA msg or NULL otherwise*/
  ,tdsrrc_PLMN_Identity *primary_plmn_Identity
)
{
  /* Send the CN info to MM */

  mm_cmd_type *mm_buf_ptr = NULL;
  uint32 num_cn_domains = 0;
  unsigned char  *tdsdata_ptr;
  uint32          i;
  tdsrrc_int_cell_id_type cell_id = 0;
  rrc_plmn_identity_type plmn_id;
 
 
                          /* Local pointer to CN Domain Information Info */

  if(cn_information_info_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"NULL Pointer for CN Information");
    return(mm_buf_ptr);
  }

  /* Get a buffer from MM task */
  if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_SERVICE_IND_FROM_OTA_MSG)) == NULL)
  {
    ERR_FATAL("No MM buffer for CSP to MM cmd 0x%x",RRC_SERVICE_IND_FROM_OTA_MSG,0,0);
  }

#ifdef FEATURE_DUAL_SIM
  mm_buf_ptr->cmd.rrc_service_ind.as_id = tdsrrc_get_as_id();
#endif


  /* Fill in the service status */
  if ((tdsrrcccm_get_serv_plmn_cellid(&cell_id, &plmn_id) == TDSRRC_CELL_LOSS_NO_NEW_CELL))
  {
    mm_buf_ptr->cmd.rrc_service_ind.service_status = MM_AS_NO_SERVICE;
  }
  else
  {
    mm_buf_ptr->cmd.rrc_service_ind.service_status = MM_AS_SERVICE_AVAILABLE;
  }

  mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.is_cell_id_valid = FALSE;

  if(mm_buf_ptr->cmd.rrc_service_ind.service_status == MM_AS_SERVICE_AVAILABLE)
  {    

    if (tdsrrc_ccm_cell_camping_info.is_curr_cell_id_valid)
    {
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.is_cell_id_valid = TRUE;
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.cell_id = cell_id;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Reporting Cell_ID :%d to NAS",cell_id);
    }

    /* Fill the cell access */
    mm_buf_ptr->cmd.rrc_service_ind.service_info.cell_access =
                                                         tdsrrc_csp_int_data.curr_cell_access_status;
     TDSRRC_MSG1(MSG_LEGACY_HIGH,"Cell Access Info when ccm build service Ind with nas CN info= %d ",mm_buf_ptr->cmd.rrc_service_ind.service_info.cell_access);	

#ifdef FEATURE_DSAC
    if(tdsrrc_csp_int_data.is_dsac_valid == TRUE)
    {
       mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present  = 
                                                 tdsrrc_csp_int_data.is_dsac_valid;
       mm_buf_ptr->cmd.rrc_service_ind.service_info.cs_cell_access = 
                                                 tdsrrc_csp_int_data.curr_cs_cell_access;
       mm_buf_ptr->cmd.rrc_service_ind.service_info.ps_cell_access = 
                                                 tdsrrc_csp_int_data.curr_ps_cell_access;
       TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: DSAC, CS and PS cell access values are %d,  %d, %d", 
                        mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present,
                        mm_buf_ptr->cmd.rrc_service_ind.service_info.cs_cell_access,
                        mm_buf_ptr->cmd.rrc_service_ind.service_info.ps_cell_access);
    }
    else
    {
      mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present = tdsrrc_csp_int_data.is_dsac_valid;
      mm_buf_ptr->cmd.rrc_service_ind.service_info.cell_access = tdsrrc_csp_int_data.curr_cell_access_status;
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"DSAC: DSAC and cell access values are %d,%d",mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present,mm_buf_ptr->cmd.rrc_service_ind.service_info.cell_access);
    }

    if(tdsrrc_csp_int_data.is_dsac_valid == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC info is valid for CCM");
      tdsrrc_ccm_cell_camping_info.curr_dsac_present = tdsrrc_csp_int_data.is_dsac_valid;
      tdsrrc_ccm_cell_camping_info.curr_cs_cell_access =   
                                            mm_buf_ptr->cmd.rrc_service_ind.service_info.cs_cell_access;
      tdsrrc_ccm_cell_camping_info.curr_ps_cell_access =   
                                 mm_buf_ptr->cmd.rrc_service_ind.service_info.ps_cell_access;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: DSAC, CS and PS access values in ccm cell camp info %d,  %d, %d", 
                                tdsrrc_ccm_cell_camping_info.curr_dsac_present,
                                tdsrrc_ccm_cell_camping_info.curr_cs_cell_access,
                                tdsrrc_ccm_cell_camping_info.curr_ps_cell_access);
    }
    else
    {
       TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC info is not valid for CCM");
       tdsrrc_ccm_cell_camping_info.curr_dsac_present = tdsrrc_csp_int_data.is_dsac_valid;
    }
#endif
    /* Fill the PLMN information */
    mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_type = RRC_GSM_MAP_PLMN_SEL;

    mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info_incl = FALSE;

    /*lint -e737*/
    /*lint -e650*/
    
    if(/*Check to see if Primary PLMN I.D is present */
       ((primary_plmn_Id_present  == TRUE)&&
        (primary_plmn_Identity != NULL)
       )
       ||/*If not then check to see if common PLMN I.D is present */
       (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cn_information_info_ptr,
       tdsrrc_CN_InformationInfo,plmn_Identity))
      )
    {
      /*lint +e737*/
      /*lint +e650*/
      /*If primary PLMN I.D is present then this should be considered 
       *  over common PLMN I.D.Since common PLMN I.D is optional field
       *  so it may not be included. In theses cases if primary PLMN I.D present
       *  honor it & send to MM
       */
      if(primary_plmn_Id_present  == TRUE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS:Primary PLMN I.D present in OTA message");
        tdsrrc_translate_gsm_map_plmn_id 
         (primary_plmn_Identity,
          &mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_id
         );
      }
      else
      {
        tdsrrc_translate_gsm_map_plmn_id 
         (&cn_information_info_ptr->plmn_Identity,
          &mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_id
         );
      }
      /* Update the current requested PLMN with the new PLMN */
      tdsrrc_csp_int_data.curr_req_plmn.plmn_id  = 
        mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_id;

      tdsrrc_print_plmn_id(tdsrrc_csp_int_data.curr_req_plmn.plmn_id);

    }
    else
    {
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[0]= 0xFF;
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[1]= 0xFF;
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[2]= 0xFF;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: Set mcc[0]=0x%x, mcc[1]=0x%x, mcc[2]=0x%x in RRC_SERVICE_IND_FROM_OTA_MSG ",
                                               mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[0],
                                               mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[1],
                                               mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[2]);
    }

    /* Fill in the CN common GSM-MAP NAS system information now */                                                
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cn_information_info_ptr,
		tdsrrc_CN_InformationInfo,cn_CommonGSM_MAP_NAS_SysInfo))
    {
      mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info_incl = TRUE;
      mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info.length =
      TDSRRC_GET_OCTET_STRING_LEN(cn_information_info_ptr->cn_CommonGSM_MAP_NAS_SysInfo);

      tdsdata_ptr = TDSRRC_GET_OCTET_STRING_PTR(cn_information_info_ptr->cn_CommonGSM_MAP_NAS_SysInfo);

      for( i = 0; i < TDSRRC_GET_OCTET_STRING_LEN(cn_information_info_ptr->cn_CommonGSM_MAP_NAS_SysInfo); i ++)
      {
        mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info.value[i] =
         *tdsdata_ptr;

        tdsdata_ptr ++;
      }
      /*Store the LAC from the OTA in global variable tdslac_in_ota*/	 	  
      if(tdsrrccsp_get_lac_id_in_dch(&(mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info)) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not able to get LAC from CN Info.");
      }	  
    }

    /* Fill in the CN domain specific NAS system info now */

    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cn_information_info_ptr,tdsrrc_CN_InformationInfo,
		cn_DomainInformationList))
    {
      uint32 idx =0;
     

      do
      {
        if(tdsrrcccm_fill_nas_cn_specific_info(cn_information_info_ptr->cn_DomainInformationList.elem[idx],
            &mm_buf_ptr->cmd.rrc_service_ind.service_info.
             cn_domain_info[num_cn_domains]) == FAILURE)
        {
          mm_free_cmd_buf(mm_buf_ptr);
          return((mm_cmd_type *)NULL);
        }
        num_cn_domains ++;
        idx++;

      }while((cn_information_info_ptr->cn_DomainInformationList.n > idx) &&
            (num_cn_domains < RRC_MAX_CN_DOMAINS));
      /* Restore the head pointer of the linked list */
#ifdef FEATURE_TDSCDMA_MODEM_STATISTICS
      tdsrrccsp_get_rac_id_in_dch(&(cn_information_info_ptr->cn_DomainInformationList));
#endif
    }

    mm_buf_ptr->cmd.rrc_service_ind.service_info.num_cn_domains =
      num_cn_domains;

  } /* Service is available */

  /* Put the command on MM queue */
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Built MM cmd 0x%x",RRC_SERVICE_IND_FROM_OTA_MSG);
  tdsrrcccm_backup_service_ind(RRC_SERVICE_IND_FROM_OTA_MSG, mm_buf_ptr);

  return(mm_buf_ptr);
} /* tdsrrcccm_buid_service_ind_with_nas_cn_info */

/*===========================================================================

FUNCTION          RRC_CCM_SEND_NAS_CN_INFO

DESCRIPTION       This function is responsible for sending the NAS specific
                  CN information to MM.

DEPENDENCIES
                  

RETURN VALUE
                  SUCCESS: Able to send RRC_SERVICE_IND_FROM_OTA_MSG successfully.
                  FAILURE: Not able to send due to any of the multiple reasons.

SIDE EFFECTS      None.
===========================================================================*/
uecomdef_status_e_type
tdsrrcccm_send_nas_cn_info
(
  tdsrrc_CN_InformationInfo* cn_information_info_ptr
   /*Boolean used to indicate primary plmn i.d is present in OTA msg*/
  ,boolean primary_plmn_Id_present
   /*pointer to primary plmn i.d if present in OTA msg or NULL otherwise*/
  ,tdsrrc_PLMN_Identity *primary_plmn_Identity
)
{
  /* Send the CN info to MM */

  mm_cmd_type *mm_buf_ptr;
  uint32 num_cn_domains = 0;
  unsigned char  *tdsdata_ptr;
  uint32          i;
  tdsrrc_int_cell_id_type cell_id = 0;
  rrc_plmn_identity_type plmn_id;
 
 

  if(cn_information_info_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"NULL Pointer for CN Information");
    return(FAILURE);
  }

  /* Get a buffer from MM task */
  if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_SERVICE_IND_FROM_OTA_MSG)) == NULL)
  {
    ERR_FATAL("No MM buffer for CSP to MM cmd 0x%x",RRC_SERVICE_IND_FROM_OTA_MSG,0,0);
  }

#ifdef FEATURE_DUAL_SIM
  mm_buf_ptr->cmd.rrc_service_ind.as_id = tdsrrc_get_as_id();
#endif


  /* Fill in the service status */
  if ((tdsrrcccm_get_serv_plmn_cellid(&cell_id, &plmn_id) == TDSRRC_CELL_LOSS_NO_NEW_CELL))
  {
    mm_buf_ptr->cmd.rrc_service_ind.service_status = MM_AS_NO_SERVICE;
  }
  else
  {
    mm_buf_ptr->cmd.rrc_service_ind.service_status = MM_AS_SERVICE_AVAILABLE;
  }

  mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.is_cell_id_valid = FALSE;

  if(mm_buf_ptr->cmd.rrc_service_ind.service_status == MM_AS_SERVICE_AVAILABLE)
  {

    if (tdsrrc_ccm_cell_camping_info.is_curr_cell_id_valid)
    {
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.is_cell_id_valid = TRUE;
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.cell_id = cell_id;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Reporting Cell_ID :%d to NAS",cell_id);
    }

    /* Fill the cell access */
    mm_buf_ptr->cmd.rrc_service_ind.service_info.cell_access =
    tdsrrc_csp_int_data.curr_cell_access_status;

#ifdef FEATURE_DSAC
    if(tdsrrc_csp_int_data.is_dsac_valid == TRUE)
    {
       TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC info is present");
       mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present  = 
                                             tdsrrc_csp_int_data.is_dsac_valid;
       mm_buf_ptr->cmd.rrc_service_ind.service_info.cs_cell_access = 
                                             tdsrrc_csp_int_data.curr_cs_cell_access;
       mm_buf_ptr->cmd.rrc_service_ind.service_info.ps_cell_access = 
                                             tdsrrc_csp_int_data.curr_ps_cell_access;
       TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: DSAC, CS and PS cell access values  are %d,  %d, %d",
                                    mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present,
                                    mm_buf_ptr->cmd.rrc_service_ind.service_info.cs_cell_access,
                                    mm_buf_ptr->cmd.rrc_service_ind.service_info.ps_cell_access);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC info is not present");
      mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present = tdsrrc_csp_int_data.is_dsac_valid;
      mm_buf_ptr->cmd.rrc_service_ind.service_info.cell_access = tdsrrc_csp_int_data.curr_cell_access_status;
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"DSAC: DSAC and cell access values  are %d,%d",mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present,mm_buf_ptr->cmd.rrc_service_ind.service_info.cell_access);
    }

    if(tdsrrc_csp_int_data.is_dsac_valid == TRUE) 
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC info is valid for CCM");
      tdsrrc_ccm_cell_camping_info.curr_dsac_present = 
                           tdsrrc_csp_int_data.is_dsac_valid;
      tdsrrc_ccm_cell_camping_info.curr_cs_cell_access =   
                           mm_buf_ptr->cmd.rrc_service_ind.service_info.cs_cell_access;
      tdsrrc_ccm_cell_camping_info.curr_ps_cell_access =   
                            mm_buf_ptr->cmd.rrc_service_ind.service_info.ps_cell_access;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: DSAC, CS and PS Access values in ccm cell camp info %d,  %d, %d", 
                            tdsrrc_ccm_cell_camping_info.curr_dsac_present,
                            tdsrrc_ccm_cell_camping_info.curr_cs_cell_access,
                            tdsrrc_ccm_cell_camping_info.curr_ps_cell_access);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC info is not valid for CCM");	  
      tdsrrc_ccm_cell_camping_info.curr_dsac_present = tdsrrc_csp_int_data.is_dsac_valid;
    }
#endif  
    /* Fill the PLMN information */
    mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_type = RRC_GSM_MAP_PLMN_SEL;

    mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info_incl = FALSE;

    /*lint -e737*/
    /*lint -e650*/
    if(/*Check to see if Primary PLMN I.D is present */
       ((primary_plmn_Id_present  == TRUE)&&
        (primary_plmn_Identity != NULL)
       )
       ||/*If not then check to see if common PLMN I.D is present */
       (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cn_information_info_ptr,
       tdsrrc_CN_InformationInfo,plmn_Identity))
      )
    {
      /*lint +e737*/
      /*lint +e650*/
      /*If primary PLMN I.D is present then this should be considered 
       *  over common PLMN I.D.Since common PLMN I.D is optional field
       *  so it may not be included. In theses cases if primary PLMN I.D present
       *  honor it & send to MM
       */
      if(primary_plmn_Id_present  == TRUE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS:Primary PLMN I.D present in OTA message");
        tdsrrc_translate_gsm_map_plmn_id 
         (primary_plmn_Identity,
          &mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_id
         );
      }
      else
      {
        tdsrrc_translate_gsm_map_plmn_id 
         (&cn_information_info_ptr->plmn_Identity,
          &mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_id
         );
      }
      /* Update the current requested PLMN with the new PLMN */
      tdsrrc_csp_int_data.curr_req_plmn.plmn_id  = 
        mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_id;

      tdsrrc_print_plmn_id(tdsrrc_csp_int_data.curr_req_plmn.plmn_id);
  
    }
    else
    {
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[0]= 0xFF;
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[1]= 0xFF;
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[2]= 0xFF;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: Set mcc[0]=0x%x, mcc[1]=0x%x, mcc[2]=0x%x in RRC_SERVICE_IND_FROM_OTA_MSG ",
                                               mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[0],
                                               mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[1],
                                               mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[2]);
    }

    /* Fill in the CN common GSM-MAP NAS system information now */                                                
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cn_information_info_ptr,
		tdsrrc_CN_InformationInfo,cn_CommonGSM_MAP_NAS_SysInfo))
    {
      mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info_incl = TRUE;
      mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info.length =
      TDSRRC_GET_OCTET_STRING_LEN(cn_information_info_ptr->cn_CommonGSM_MAP_NAS_SysInfo);

      tdsdata_ptr = TDSRRC_GET_OCTET_STRING_PTR(cn_information_info_ptr->cn_CommonGSM_MAP_NAS_SysInfo);

      for( i = 0; i < TDSRRC_GET_OCTET_STRING_LEN(cn_information_info_ptr->cn_CommonGSM_MAP_NAS_SysInfo); i ++)
      {
        mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info.value[i] =
         *tdsdata_ptr;

        tdsdata_ptr ++;
      }
      /*Store the LAC from the OTA in global variable tdslac_in_ota*/		  
      if(tdsrrccsp_get_lac_id_in_dch(&(mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info)) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not able to get LAC from CN Info.");
      } 	  
    }

    /* Fill in the CN domain specific NAS system info now */

    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cn_information_info_ptr,
		tdsrrc_CN_InformationInfo,cn_DomainInformationList))
    {
      uint32 idx =0;

      do
      {
        if(tdsrrcccm_fill_nas_cn_specific_info(cn_information_info_ptr->cn_DomainInformationList.elem[idx],
            &mm_buf_ptr->cmd.rrc_service_ind.service_info.
             cn_domain_info[num_cn_domains]) == FAILURE)
        {
          mm_free_cmd_buf(mm_buf_ptr);
          return(FAILURE);
        }
        num_cn_domains ++;
        

      }while((cn_information_info_ptr->cn_DomainInformationList.n > idx) &&
            (num_cn_domains < RRC_MAX_CN_DOMAINS));
      /* Restore the head pointer of the linked list */

#ifdef FEATURE_TDSCDMA_MODEM_STATISTICS
      tdsrrccsp_get_rac_id_in_dch(&(cn_information_info_ptr->cn_DomainInformationList));
#endif
    }

    mm_buf_ptr->cmd.rrc_service_ind.service_info.num_cn_domains =
      num_cn_domains;

  } /* Service is available */

  /* Put the command on MM queue */
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending MM cmd 0x%x",RRC_SERVICE_IND_FROM_OTA_MSG);
  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
      tdscfa_mm_put_cmd(mm_buf_ptr);
  }

  tdsrrcccm_backup_service_ind(RRC_SERVICE_IND_FROM_OTA_MSG, mm_buf_ptr);
  
  mm_put_cmd(mm_buf_ptr);

  return(SUCCESS);
} /* tdsrrcccm_send_nas_cn_info */
/*===========================================================================

FUNCTION          RRC_CCM_SEND_NAS_CN_INFO_FULL

DESCRIPTION       This function is responsible for sending the NAS specific
                  CN_FULL information to MM.

DEPENDENCIES
                  

RETURN VALUE
                  SUCCESS: Able to send RRC_SERVICE_IND_FROM_OTA_MSG successfully.
                  FAILURE: Not able to send due to any of the multiple reasons.

SIDE EFFECTS      None.
===========================================================================*/
uecomdef_status_e_type
tdsrrcccm_send_nas_cn_info_full
(
  tdsrrc_CN_InformationInfoFull* cn_information_info_full_ptr
   /*Boolean used to indicate primary plmn i.d is present in OTA msg*/
  ,boolean primary_plmn_Id_present
   /*pointer to primary plmn i.d if present in OTA msg or NULL otherwise*/
  ,tdsrrc_PLMN_Identity *primary_plmn_Identity
)
{
  /* Send the CN info to MM */

  mm_cmd_type *mm_buf_ptr;
  uint32 num_cn_domains = 0;
  unsigned char  *tdsdata_ptr;
  uint32          i;
  tdsrrc_int_cell_id_type cell_id = 0;
  rrc_plmn_identity_type plmn_id;

  

  if(cn_information_info_full_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"NULL Pointer for CN Information Full");
    return(FAILURE);
  }

  /* Get a buffer from MM task */
  if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_SERVICE_IND_FROM_OTA_MSG)) == NULL)
  {
    ERR_FATAL("No MM buffer for CSP to MM cmd 0x%x",RRC_SERVICE_IND_FROM_OTA_MSG,0,0);
  }

#ifdef FEATURE_DUAL_SIM
  mm_buf_ptr->cmd.rrc_service_ind.as_id = tdsrrc_get_as_id();
#endif


  /* Fill in the service status */
  if ((tdsrrcccm_get_serv_plmn_cellid(&cell_id, &plmn_id) == TDSRRC_CELL_LOSS_NO_NEW_CELL))
  {
    mm_buf_ptr->cmd.rrc_service_ind.service_status = MM_AS_NO_SERVICE;
  }
  else
  {
    mm_buf_ptr->cmd.rrc_service_ind.service_status = MM_AS_SERVICE_AVAILABLE;
  }

  mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.is_cell_id_valid = FALSE;

  if(mm_buf_ptr->cmd.rrc_service_ind.service_status == MM_AS_SERVICE_AVAILABLE)
  {    

    if (tdsrrc_ccm_cell_camping_info.is_curr_cell_id_valid)
    {
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.is_cell_id_valid = TRUE;
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.cell_id = cell_id;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Reporting Cell_ID :%d to NAS",cell_id);
    }

    /* Fill the cell access */
    mm_buf_ptr->cmd.rrc_service_ind.service_info.cell_access =
                                                            tdsrrc_csp_int_data.curr_cell_access_status;
#ifdef FEATURE_DSAC
    if(tdsrrc_csp_int_data.is_dsac_valid == TRUE)
    {
       TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC info is present");
       mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present  =  
                                                  tdsrrc_csp_int_data.is_dsac_valid;
       mm_buf_ptr->cmd.rrc_service_ind.service_info.cs_cell_access = 
                                                  tdsrrc_csp_int_data.curr_cs_cell_access;
       mm_buf_ptr->cmd.rrc_service_ind.service_info.ps_cell_access = 
                                                  tdsrrc_csp_int_data.curr_ps_cell_access;
       TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: DSAC, CS and PS cell access values are %d,  %d, %d", 
                                          mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present,
                                          mm_buf_ptr->cmd.rrc_service_ind.service_info.cs_cell_access,
                                          mm_buf_ptr->cmd.rrc_service_ind.service_info.ps_cell_access);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC info is not valid");
      mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present = 
                                                tdsrrc_csp_int_data.is_dsac_valid;
      mm_buf_ptr->cmd.rrc_service_ind.service_info.cell_access = 
                                               tdsrrc_csp_int_data.curr_cell_access_status;
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"DSAC: DSAC and cell access values are %d,%d",mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present,mm_buf_ptr->cmd.rrc_service_ind.service_info.cell_access);
    }

    if(tdsrrc_csp_int_data.is_dsac_valid == TRUE) 
    {
       TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC info is valid for CCM");
       tdsrrc_ccm_cell_camping_info.curr_dsac_present = tdsrrc_csp_int_data.is_dsac_valid;
       tdsrrc_ccm_cell_camping_info.curr_cs_cell_access =   mm_buf_ptr->cmd.rrc_service_ind.service_info.cs_cell_access;
       tdsrrc_ccm_cell_camping_info.curr_ps_cell_access =   mm_buf_ptr->cmd.rrc_service_ind.service_info.ps_cell_access;
       TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: DSAC, CS and PS Access values are %d,  %d, %d", 
                                                            tdsrrc_ccm_cell_camping_info.curr_dsac_present,
                                                            tdsrrc_ccm_cell_camping_info.curr_cs_cell_access,
                                                            tdsrrc_ccm_cell_camping_info.curr_ps_cell_access);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC info is not valid for CCM");
      tdsrrc_ccm_cell_camping_info.curr_dsac_present = tdsrrc_csp_int_data.is_dsac_valid;
    }
#endif
    /* Fill the PLMN information */
    mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_type = RRC_GSM_MAP_PLMN_SEL;

    mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info_incl = FALSE;

    /*lint -e737*/
    if(/*Check to see if Primary PLMN I.D is present */
       ((primary_plmn_Id_present  == TRUE)&&
        (primary_plmn_Identity != NULL)
       )
       ||/*If not then check to see if common PLMN I.D is present */
       (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cn_information_info_full_ptr,
       tdsrrc_CN_InformationInfoFull,plmn_Identity))
     )
    {
      /*lint +e737*/
      /*If primary PLMN I.D is present then this should be considered 
       *  over common PLMN I.D.Since common PLMN I.D is optional field
       *  so it may not be included. In theses cases if primary PLMN I.D present
       *  honor it & send to MM
       */
      if(primary_plmn_Id_present  == TRUE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS:Primary PLMN I.D present in OTA message");
        tdsrrc_translate_gsm_map_plmn_id 
         (primary_plmn_Identity,
          &mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_id
         );
      }
      else
      {
        tdsrrc_translate_gsm_map_plmn_id 
         (&cn_information_info_full_ptr->plmn_Identity,
          &mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_id
         );  
      }
      /* Update the current requested PLMN with the new PLMN */
      tdsrrc_csp_int_data.curr_req_plmn.plmn_id  = 
        mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn.plmn_id;

	  tdsrrc_csp_int_data.curr_acq_entry.plmn_id = tdsrrc_csp_int_data.curr_req_plmn.plmn_id;
	  
	  /* Update the ACQ DB entry with the PLMN info received in UMI message*/
	  if(tdsrrccsp_add_acq_entry(&tdsrrc_csp_int_data.curr_acq_entry) == FAILURE)
	  {
	    ERR_FATAL("Failure adding acq. entry", 0, 0, 0);
	  }
	  
      tdsrrc_print_plmn_id(tdsrrc_csp_int_data.curr_req_plmn.plmn_id);
   
    }
    else
    {
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[0]= 0xFF;
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[1]= 0xFF;
      mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[2]= 0xFF;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: Set mcc[0]=0x%x, mcc[1]=0x%x, mcc[2]=0x%x in RRC_SERVICE_IND_FROM_OTA_MSG ",
                                               mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[0],
                                               mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[1],
                                               mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[2]);
    }

    /* Fill in the CN common GSM-MAP NAS system information now */
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(cn_information_info_full_ptr,
       tdsrrc_CN_InformationInfoFull,cn_CommonGSM_MAP_NAS_SysInfo))
    {
      mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info_incl = TRUE;
      mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info.length =
       TDSRRC_GET_OCTET_STRING_LEN(cn_information_info_full_ptr->cn_CommonGSM_MAP_NAS_SysInfo);

      tdsdata_ptr = TDSRRC_GET_OCTET_STRING_PTR(cn_information_info_full_ptr->cn_CommonGSM_MAP_NAS_SysInfo);

      for( i = 0; i < TDSRRC_GET_OCTET_STRING_LEN(cn_information_info_full_ptr->cn_CommonGSM_MAP_NAS_SysInfo); i ++)
      {
        mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info.value[i] =
         *tdsdata_ptr;

        tdsdata_ptr ++;
      }
      /*Store the LAC from the OTA in global variable tdslac_in_ota*/		  
      if(tdsrrccsp_get_lac_id_in_dch(&(mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info)) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not able to get LAC from CN Info.");
      } 	  
    }

    /* Fill in the CN domain specific NAS system info now */
    if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(cn_information_info_full_ptr,
		cn_DomainInformationListFull))
    {
      uint32 idx =0;
      do
      {
        if(tdsrrcccm_fill_nas_cn_specific_info_full(
          cn_information_info_full_ptr->cn_DomainInformationListFull.elem[idx],
          &mm_buf_ptr->cmd.rrc_service_ind.service_info.
          cn_domain_info[num_cn_domains]) == FAILURE)
        {
          mm_free_cmd_buf(mm_buf_ptr);
          return(FAILURE);
        }
        num_cn_domains ++;
        idx++;

      }while((cn_information_info_full_ptr->cn_DomainInformationListFull.n > idx) &&
            (num_cn_domains < RRC_MAX_CN_DOMAINS));
    
      

    }

    mm_buf_ptr->cmd.rrc_service_ind.service_info.num_cn_domains =
      num_cn_domains;

  } /* Service is available */

  /* Put the command on MM queue */
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending MM cmd 0x%x",RRC_SERVICE_IND_FROM_OTA_MSG);
  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
      tdscfa_mm_put_cmd(mm_buf_ptr);
  }
  tdsrrcccm_backup_service_ind(RRC_SERVICE_IND_FROM_OTA_MSG, mm_buf_ptr);
  mm_put_cmd(mm_buf_ptr);

  return(SUCCESS);
} /* tdsrrcccm_send_nas_cn_info_full */

/*===========================================================================

FUNCTION          RRC_CCM_SEND_NEW_CELL_IND

DESCRIPTION       This function is responsible for sending the New Cell Ind.
                  to Internal RRC queue.

DEPENDENCIES
                  None

RETURN VALUE
                  A status that indicates if the New Cell Ind. could be
                  sent Successfully or not.

SIDE EFFECTS      None.

===========================================================================*/
uecomdef_status_e_type  
tdsrrcccm_send_new_cell_ind(tdsrrc_new_cell_status_e_type  new_cell_camp_status,
                          tdsrrc_proc_e_type  proc_id,
                          boolean cell_same_as_before,
                          boolean inform_procedure_rce)
{
  tdsrrc_cmd_type   *csp_out_cmd_ptr;
  /* Send New Cell Ind */
  if((csp_out_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {  
    csp_out_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_NEW_CELL_IND;
    csp_out_cmd_ptr->cmd.new_cell_ind.new_cell_status = new_cell_camp_status;
    csp_out_cmd_ptr->cmd.new_cell_ind.no_ind_proc_id = proc_id;
    csp_out_cmd_ptr->cmd.new_cell_ind.cell_same_as_before = cell_same_as_before;
    csp_out_cmd_ptr->cmd.new_cell_ind.inform_procedure_rce = inform_procedure_rce;

    /* Send the Internal cmd to RRC */
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"TDS NEW_CELL_IND: cell changed(%d), status(%d), rce(%d)", 
             !cell_same_as_before, 
             new_cell_camp_status, 
             inform_procedure_rce);
    tdsrrc_put_int_cmd( csp_out_cmd_ptr );
    return(SUCCESS);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failure to get Int Cmd buf");
    return(FAILURE);
  }
} /* tdsrrcccm_send_new_cell_ind */


/*===========================================================================

FUNCTION          tdsrrcccm_send_cell_change_cnf

DESCRIPTION       This function is responsible for sending the Cell Change
                  Confirm to the passed RRC Procedure.

DEPENDENCIES
                  None

RETURN VALUE
                  A status that indicates if the Cell Change Cnf could be
                  sent Successfully or not.
SIDE EFFECTS
===========================================================================*/
static uecomdef_status_e_type 
tdsrrcccm_send_cell_change_cnf(tdsrrc_cell_change_status_e_type  cell_change_status, 
                             tdsrrc_new_cell_status_e_type  new_cell_camp_status,
                             tdsrrc_proc_e_type   proc_id,
                             tdsrrc_csp_acq_entry_type  *acq_entry_ptr)
{

  tdsrrc_cmd_type   *csp_out_cmd_ptr;

  /* Check validity of Acq Entry pointer */
  if(acq_entry_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Passed Acq Entry pointer is NULL");
    return(FAILURE);
  }

  /* Proceed further only if procedure ID is valid */
  if(proc_id < TDSMAX_NUM_OF_PROCEDURES)
  {
    /* Send Cell Change Confirm back to requesting procedure */
    if((csp_out_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
    {    
      csp_out_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CELL_CHANGE_CNF;
      csp_out_cmd_ptr->cmd.cell_change_cnf.req_proc_id = proc_id;
      csp_out_cmd_ptr->cmd.cell_change_cnf.cell_change_status = cell_change_status;
      csp_out_cmd_ptr->cmd.cell_change_cnf.new_cell_status = new_cell_camp_status;
      csp_out_cmd_ptr->cmd.cell_change_cnf.freq = acq_entry_ptr->freq;
      csp_out_cmd_ptr->cmd.cell_change_cnf.cpid = acq_entry_ptr->cpid;
      csp_out_cmd_ptr->cmd.cell_change_cnf.plmn_id = acq_entry_ptr->plmn_id;
      csp_out_cmd_ptr->cmd.cell_change_cnf.cell_id = acq_entry_ptr->cell_id;
      /* Send the Internal cmd to RRC */
      tdsrrc_put_int_cmd( csp_out_cmd_ptr );
      return(SUCCESS);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failure to get Int Cmd buf");
      return(FAILURE);
    }
  } /* Procedure ID is valid */

  else  /* Procedure ID is invalid */
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Proc ID for req procedure is invalid");
    return(FAILURE);
  }

} /* tdsrrcccm_send_cell_change_cnf */

/*===========================================================================

FUNCTION          tdsrrcccm_cell_change_to_same_cell

DESCRIPTION       This function returns a boolean that tells whether UE
                  camped on the same cell as before.
DEPENDENCIES
                  None

RETURN VALUE
                  boolean true : cell is same as before
                         false : cell is different

SIDE EFFECTS
                  None.

===========================================================================*/
boolean tdsrrcccm_cell_change_to_same_cell(void)
{
  rrc_plmn_identity_type prev_plmn_id, current_plmn_id;
  if(tdsrrc_ccm_cell_camping_info.prev_cell.is_valid_nw_sharing)
  {
    prev_plmn_id = tdsrrc_ccm_cell_camping_info.prev_cell.shared_list_of_plmns.common_plmn_id;
  }
  else
  {
    prev_plmn_id = tdsrrc_ccm_cell_camping_info.prev_cell.plmn_id;
  }

  if(tdsrrc_ccm_cell_camping_info.curr_cell.is_valid_nw_sharing)
  {
    current_plmn_id = tdsrrc_ccm_cell_camping_info.curr_cell.shared_list_of_plmns.common_plmn_id;
  }
  else
  {
    current_plmn_id = tdsrrc_ccm_cell_camping_info.curr_cell.plmn_id;
  }
  if( (tdsrrc_bcch_plmns_match(current_plmn_id,prev_plmn_id) == FALSE) ||
      (tdsrrc_ccm_cell_camping_info.curr_cell.cell_id != tdsrrc_ccm_cell_camping_info.prev_cell.cell_id) )
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell:%d is different. cur_psc:%d prev_psc:%d",
             tdsrrc_ccm_cell_camping_info.curr_cell.cell_id,
             tdsrrc_ccm_cell_camping_info.curr_cell.cpid,
             tdsrrc_ccm_cell_camping_info.prev_cell.cpid
             );
    return(FALSE);
  }
  else
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell:%d is same as before. freq:%d cpid:%d",
             tdsrrc_ccm_cell_camping_info.curr_cell.cell_id,
             tdsrrc_ccm_cell_camping_info.curr_cell.freq,
             tdsrrc_ccm_cell_camping_info.curr_cell.cpid
             );
    return(TRUE);
  }
} /* tdsrrcccm_cell_change_to_same_cell */


/*===========================================================================

FUNCTION          tdsrrcccm_handle_cell_change_success

DESCRIPTION       This function is responsible for all actions needed when
                  a Cell Change Request succeeds.
DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None.

===========================================================================*/
static void tdsrrcccm_handle_cell_change_success(void)
{
  boolean  cell_same_as_before = TRUE;
  boolean  inform_procedure_rce = FALSE;

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  tdsrrc_proc_e_type                 proc_id;
#endif

  tdsrrcrce_substate_e_type  rrcrce_substate = tdsrrcrce_get_rce_sub_state();

  rrc_plmn_identity_type prev_plmn_id, current_plmn_id;

  tdsrrc_ccm_cell_camping_info.is_curr_cell_id_valid = TRUE;

  /* Before sending new cell related cmds to other modules, change 
   * camping status to TDSRRC_NEW_CELL_SELECTED.
   */
  tdsrrc_ccm_cell_camping_info.cell_camp_status = TDSRRC_NEW_CELL_SELECTED;
  tdsrrc_ccm_cell_camping_info.status = TDSSTATUS_VALID;
  /* Re-initialise high_loading_thesh once change to new cell*/
  tdsrrcpg1_page_detect_pattern.high_loading_thesh = TDS_PAGE_PATTERN_DETECT_HIGH_LOADING_UE_NUM;


  /* Now send a Service Ind to NAS here. If the cell has been changed
   * due to a request from Cell Selection Procedure, it is left 
   * to the cell selection to decide if a SERVICE_CNF or a 
   * SERVICE_IND needs to be sent.
   */
  if(tdsrrc_ccm_cell_camping_info.req_proc_id != TDSRRC_PROCEDURE_CSP)
  {
    if(tdsrrcccm_send_mm_service_ind() == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Service ind not sent");
    }
  }

  /* Send a Cell Change Cnf with Success to the calling procedure */
  if(tdsrrcccm_send_cell_change_cnf(TDSRRC_CELL_CHANGE_SUCCESS,
                                  tdsrrc_ccm_cell_camping_info.cell_camp_status,
                                  tdsrrc_ccm_cell_camping_info.req_proc_id,
                                  &tdsrrc_ccm_cell_camping_info.curr_cell)
     == FAILURE)
  {
    ERR_FATAL("Failure to send Cell Change Cnf",0,0,0);
  }

  if(tdsrrc_ccm_cell_camping_info.prev_cell.is_valid_nw_sharing)
  {
    prev_plmn_id = tdsrrc_ccm_cell_camping_info.prev_cell.shared_list_of_plmns.common_plmn_id;
  }
  else
  {
    prev_plmn_id = tdsrrc_ccm_cell_camping_info.prev_cell.plmn_id;
  }

  if(tdsrrc_ccm_cell_camping_info.curr_cell.is_valid_nw_sharing)
  {
    current_plmn_id = tdsrrc_ccm_cell_camping_info.curr_cell.shared_list_of_plmns.common_plmn_id;
  }
  else
  {
    current_plmn_id = tdsrrc_ccm_cell_camping_info.curr_cell.plmn_id;
  }

  if( (tdsrrc_bcch_plmns_match(current_plmn_id, 
                            prev_plmn_id) == FALSE) ||
      (tdsrrc_ccm_cell_camping_info.curr_cell.cell_id != tdsrrc_ccm_cell_camping_info.prev_cell.cell_id) )
  {
    cell_same_as_before = FALSE;
  }
  else
  {
    if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_RESELECTION_SCAN)
    {
      ERR_FATAL("Same cell/PLMN id during cell reselection",0,0,0);
    }
  }
  if (!cell_same_as_before)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"cs_domain_error:reset tdsrrc_cs_domain_error");                
    tdsrrc_cs_domain_error.mo_error_count = 0;
    tdsrrc_cs_domain_error.mt_error_count = 0;
  }
  if(tdsrrc_ccm_cell_camping_info.rce_found_status_not_camping)
  {
    inform_procedure_rce = TRUE;
    tdsrrc_ccm_cell_camping_info.rce_found_status_not_camping = FALSE;
  }

  if(inform_procedure_rce || 
     ( (rrcrce_substate == TDSRRCRCE_WAIT_FOR_SIB7_CURRENT) ||
       (rrcrce_substate == TDSRRCRCE_WAIT_FOR_RRC_CONN_SETUP) ||
       (rrcrce_substate == TDSRRCRCE_WAIT_FOR_RESELCT_COMPL) ||
       (rrcrce_substate == TDSRRCRCE_WAIT_FOR_REDIRECT_CNF) )
    )
  {
    if(tdsrrcrce_get_rrc_est_cause() != RRC_EST_EMERGENCY_CALL)
    {
      // Compare the currently camped RAI vs the one given in EST_REQ
      if(tdsrrcrce_check_for_rai_mismatch(tdsrrc_ccm_cell_camping_info.curr_cell.plmn_id))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LA/RA mismatch,Initialize RCE");
        inform_procedure_rce = FALSE;
        tdsrrcrce_force_initialize(TRUE);
      }
    }
  }

  // CR 352866 begin
  if ((rrcrce_substate == TDSRRCRCE_WAIT_NEW_CELL_IND)||
      (rrcrce_substate == TDSRRCRCE_WAIT_NEW_CELL_IND_SIB7))
  {
    inform_procedure_rce = TRUE;
  }
  // CR 352866 end

  /* Send an Internal New Cell Ind only if Cell Change is not due to
   * transition from CELL_DCH to CELL_FACH / CELL_FACH to PCH .
   * For CELL_FACH to CELL_FACH, send New Cell Ind to CU if OOS 
   * is triggered.
   */
  if((tdsrrc_ccm_cell_camping_info.cell_change_type != TDSRRC_CHANGE_CELL_DCH_TO_CELL_FACH) &&
     (tdsrrc_ccm_cell_camping_info.cell_change_type != TDSRRC_CHANGE_CELL_DCH_TO_CELL_PCH) &&
     (tdsrrc_ccm_cell_camping_info.cell_change_type != TDSRRC_CHANGE_CELL_DCH_TO_URA_PCH)
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
     && (tdsrrc_ccm_cell_camping_info.cell_change_type != TDSRRC_CHANGE_CELL_FACH_TO_CELL_PCH)
     && (tdsrrc_ccm_cell_camping_info.cell_change_type != TDSRRC_CHANGE_CELL_FACH_TO_URA_PCH)
     && (!((tdsrrc_ccm_cell_camping_info.cell_change_type == TDSRRC_CHANGE_CELL_FACH_TO_CELL_FACH) &&
           (tdsrrcrb_check_if_proc_waiting_for_new_cell_ind(&proc_id) == TRUE )))
#endif
    )
  {
    if((cell_same_as_before == FALSE) || 
       (inform_procedure_rce == TRUE) ||
       (tdsrrc_ccm_cell_camping_info.cell_change_type == TDSRRC_CHANGE_RETURN_TO_SERVICE_AREA) 
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
       || ((tdsrrc_ccm_cell_camping_info.cell_change_type == TDSRRC_CHANGE_CELL_FACH_TO_CELL_FACH)
           && ((tdsrrcllc_get_toc_usage() == TDSTOC_FOR_OOS_WITH_DCH_INFO )||
               (tdsrrcllc_get_toc_usage() == TDSTOC_FOR_OOS_WITHOUT_DCH_INFO )))
#endif
       )
    {
      /* Send an Indication ONLY when either the new cell is different
       * than the last camped on cell OR if RCE wants an Indication.
       */
      /* We don't want the procedure initiating this change to get an IND 
       * It will get a CNF and an extra IND might be confusing.
       */
      if(tdsrrcccm_send_new_cell_ind(TDSRRC_NEW_CELL_SELECTED, 
                                   tdsrrc_ccm_cell_camping_info.req_proc_id,
                                   cell_same_as_before,
                                   inform_procedure_rce)
         == FAILURE)
      {
        ERR_FATAL("Failure to send New Cell Ind",0,0,0);
      }
    }
  } /* If Cell Change is not due to CELL_DCH to CELL_FACH transition */



#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  /* For CELL_FACH to CELL_FACH send New Cell Ind to the procedure which initiated
   * the cell selection due to redirection, as there will not be any State Change Ind
   * to take care of.
   */
  if((tdsrrc_ccm_cell_camping_info.cell_change_type == TDSRRC_CHANGE_CELL_FACH_TO_CELL_FACH)
     && (tdsrrcrb_check_if_proc_waiting_for_new_cell_ind(&proc_id) == TRUE ))
  {
    tdsrrcccm_propagate_new_cell_to_waiting_proc(proc_id, cell_same_as_before, inform_procedure_rce);
  }
#endif /* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

#ifdef FEATURE_RAT_PRIORITY_LIST
  if ((tdsrrc_ccm_cell_camping_info.cell_change_type == TDSRRC_CHANGE_CELL_SELECTION)
#ifdef FEATURE_WRLF_SYSTEM_SEL
      || (TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
#endif
     )
  {
    tdsrrccsp_update_mode_band_pref_after_succ_camping();
  }
#endif

} /* tdsrrcccm_handle_cell_change_success */


/*===========================================================================

FUNCTION          tdsrrcccm_handle_cell_change_failure

DESCRIPTION       This function is responsible for all actions needed when
                  a Cell Change Request fails due to any of the multiple 
                  reasons.
DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None.

===========================================================================*/
static void tdsrrcccm_handle_cell_change_failure(tdsrrc_proc_e_type  proc_id, 
                                        tdsrrc_csp_acq_entry_type  *acq_entry_ptr,
                                        tdsrrc_cell_change_status_e_type cell_change_status)
{
  /* Set the camping Status to TDSRRC_CELL_LOSS_NO_NEW_CELL */
  tdsrrc_ccm_cell_camping_info.cell_camp_status = TDSRRC_CELL_LOSS_NO_NEW_CELL;
  tdsrrc_ccm_cell_camping_info.status = TDSSTATUS_VALID;
  
  /* Send a Cell Change CNF with Failure to the passed Procedure ID */
  if(tdsrrcccm_send_cell_change_cnf(cell_change_status,
                                  tdsrrc_ccm_cell_camping_info.cell_camp_status,
                                  proc_id, acq_entry_ptr)
     == FAILURE)
  {
    ERR_FATAL("Failure to send Cell Change Cnf",0,0,0);
  }

  /* Let us send a New Cell Ind so that those procedures that want to be
   * informed of a loss of service will get to know.
   */
  if(tdsrrcccm_send_new_cell_ind(TDSRRC_CELL_LOSS_NO_NEW_CELL, proc_id, FALSE, FALSE)
     == FAILURE)
  {
    ERR_FATAL("Failure to send New Cell Ind",0,0,0);
  }

} /* tdsrrcccm_handle_cell_change_failure */


/*===========================================================================

FUNCTION        tdsrrcccm_update_state_change

DESCRIPTION
                This function processes State Change Indication.
  
DEPENDENCIES
                None

RETURN VALUE
                None.

SIDE EFFECTS
                None

===========================================================================*/
static tdsrrc_ccm_substate_e_type
tdsrrcccm_update_state_change( tdsrrc_cmd_type *cmd_ptr )
{
  tdsrrc_ccm_substate_e_type next_substate = TDSRRC_CCM_IDLE;
  if(cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_CELL_DCH)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"State change to DCH rxed,reset resel vars");
    /* Copy previous cell to current cell since we are ready to accept
     * next cell change request.
     */
    tdsrrc_ccm_cell_camping_info.status = TDSSTATUS_VALID;
    tdsrrc_ccm_cell_camping_info.curr_cell = tdsrrc_ccm_cell_camping_info.prev_cell;
    tdsrrc_ccm_cell_camping_info.curr_cell_access = 
                   tdsrrc_ccm_cell_camping_info.prev_cell_access;
#ifdef FEATURE_DSAC
    tdsrrc_ccm_cell_camping_info.curr_dsac_present= 
                         tdsrrc_ccm_cell_camping_info.prev_dsac_present;
    tdsrrc_ccm_cell_camping_info.curr_cs_cell_access = 
                         tdsrrc_ccm_cell_camping_info.prev_cs_cell_access;
    tdsrrc_ccm_cell_camping_info.curr_ps_cell_access = 
                         tdsrrc_ccm_cell_camping_info.prev_ps_cell_access;
    TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: Back up of DSAC info during  <ccm> update state change !");
#endif 
  }
  else if(cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"State change to Idle,going back to CCM_IDLE");
    /* The status valid is set to FALSE till a Cell Change Req is processed.
     */
    tdsrrc_ccm_cell_camping_info.status = TDSSTATUS_INVALID_CELL_TO_BE_CAMPED;
    tdsrrc_ccm_cell_camping_info.cell_camp_status = TDSRRC_CELL_LOSS_NO_NEW_CELL;
  
    /* Set to FALSE, the boolean that indicates that RCE found status as "Not Camping"*/
    tdsrrc_ccm_cell_camping_info.rce_found_status_not_camping = FALSE;
    /* Set the Cell change requesting procedure ID to Invalid value */
    tdsrrc_ccm_cell_camping_info.req_proc_id = TDSMAX_NUM_OF_PROCEDURES;
  
    /* Initialize the state of RRC CCM to IDLE */
    next_substate = TDSRRC_CCM_IDLE;
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unexpected state change to %d",cmd_ptr->cmd.state_change_ind.new_state);
  }

  return next_substate;

} /* rrc_ccm_update_for_state_change */


/*===========================================================================

FUNCTION          tdsrrcccm_wait_cell_reselection_handler

DESCRIPTION       This function is responsible for handling all commands 
                  in TDSRRC_CCM_WAIT_CELL_RESELECTION_CNF substate.
DEPENDENCIES
                  None

RETURN VALUE
                  Next CCM substate.

SIDE EFFECTS
                  None.
===========================================================================*/
static tdsrrc_ccm_substate_e_type
tdsrrcccm_wait_cell_reselection_handler( tdsrrc_cmd_type  *cmd_ptr)
{
  tdsrrc_ccm_substate_e_type next_substate = TDSRRC_CCM_WAIT_CELL_RESELECTION_CNF;

  /* To post a command TDSRRC_CELL_SELECTED_MEAS_REQ to measurement procedure */
  tdsrrc_cmd_type* rrcmeas_cmd_ptr;
  unsigned long sib7_timer_val;

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_STATE_CHANGE_IND:
      next_substate = tdsrrcccm_update_state_change(cmd_ptr);
      break;

    case TDSRRC_CELL_RESELECTION_CNF:
      /* Reselection confirm received */
      if(cmd_ptr->cmd.cell_resel_cnf.status == FAILURE)
      {
        /* Cell Reselection failed - send CELL_CHANGE_CNF with failure to CSP */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quit Cell Change (Not Camped)- Cell Resel failed");
    
        tdsrrcccm_handle_cell_change_failure(tdsrrc_ccm_cell_camping_info.req_proc_id,
                                           &tdsrrc_ccm_cell_camping_info.curr_cell,
                                           TDSRRC_CELL_CHANGE_FAILURE_OTHER);

        tdsrrc_ccm_cell_camping_info.curr_cell =  
                        tdsrrc_ccm_cell_camping_info.prev_cell;
                  
        tdsrrc_ccm_cell_camping_info.curr_cell_access = 
                          tdsrrc_ccm_cell_camping_info.prev_cell_access;
                  
                  
#ifdef FEATURE_DSAC
        tdsrrc_ccm_cell_camping_info.curr_dsac_present = 
                      tdsrrc_ccm_cell_camping_info.prev_dsac_present;
                  
        tdsrrc_ccm_cell_camping_info.curr_cs_cell_access = 
                      tdsrrc_ccm_cell_camping_info.prev_cs_cell_access;
                  
        tdsrrc_ccm_cell_camping_info.curr_ps_cell_access = 
                      tdsrrc_ccm_cell_camping_info.prev_ps_cell_access;   
#endif 


      } /* Cell reselection failure */
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_MED,"Cell change in RRC state %d",tdsrrc_get_state());

        /* Send a Cell parameter Req to L1 */
        if(tdsrrccsp_send_l1_cmd(TDSRRC_PROCEDURE_CCM, TDSL1_CPHY_CELL_PARM_REQ, &tdsrrc_ccm_cell_camping_info.curr_cell)
          == FAILURE)
        {
          ERR_FATAL("Failed to send L1/LLC cmd %d",TDSL1_CPHY_CELL_PARM_REQ,0,0);
        }
      
        /* Send a Cell measurement Req to L1 */
        if((rrcmeas_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
        {
          rrcmeas_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CELL_SELECTED_MEAS_REQ;
          rrcmeas_cmd_ptr->cmd.cell_selected_meas_req.gtow_ho_active = FALSE;
          tdsrrc_put_int_cmd(rrcmeas_cmd_ptr);
        }
        else
        {
          ERR_FATAL("Couldn't get internal command buffer",0,0,0);
        }

        tdsrrcccm_handle_cell_change_success();
      /* TDSRRCTMR_SIB7_UPDATE_WAIT_TIMER only starts if tds_nv_sib7_exp_time_factor is not 0.
         Only when sib confirm received would set serving cell index.
      */
      if (tds_nv_sib7_exp_time_factor > 0) 
      {
        /* Get default sib7 expiration timer value from sib7 */
        sib7_timer_val = tdsrrcsib_get_sib7_exp_timer_value();

        /* We need to multiply a NV value to start NV controlled sib7 wait timer.
           Default NV value is 1. */
        sib7_timer_val *= tds_nv_sib7_exp_time_factor;
        tdsrrctmr_start_timer(TDSRRCTMR_SIB7_UPDATE_WAIT_TIMER, sib7_timer_val);
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB7_UPDATE_WAIT_TIMER started,timer_val = %d",sib7_timer_val);
      }
      else
      {
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB7_UPDATE_WAIT_TIMER not started,tds_nv_sib7_exp_time_factor = %d",tds_nv_sib7_exp_time_factor);
      }

#if (!(defined(T_WINNT)))
        /* Log the state change with QXDM */
        tdsrrcccm_log_cell_info();
#endif /* (!(defined(T_WINNT))) */

      } /* Successful cell reselection */

      next_substate = TDSRRC_CCM_IDLE;

    break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Unexpected Cmd 0x%x in RRC CCM substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_ccm_substate);
      break;

  }

  return(next_substate);

} /* tdsrrcccm_wait_cell_reselection_handler */


/*===========================================================================

FUNCTION          tdsrrcccm_wait_chan_config_handler

DESCRIPTION       This function is responsible for handling all commands 
                  in TDSRRC_CCM_WAIT_CHAN_CONFIG_CNF substate.
DEPENDENCIES
                  None

RETURN VALUE
                  Next CCM substate.

SIDE EFFECTS
                  None.
===========================================================================*/
static tdsrrc_ccm_substate_e_type
tdsrrcccm_wait_chan_config_handler( tdsrrc_cmd_type  *cmd_ptr )
{
  tdsrrc_cmd_type* rrcmeas_cmd_ptr;  
  tdsrrc_ccm_substate_e_type next_substate = TDSRRC_CCM_WAIT_CHAN_CONFIG_CNF;
  tdsrrc_cell_change_status_e_type cell_change_status;

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_STATE_CHANGE_IND:
      next_substate = tdsrrcccm_update_state_change(cmd_ptr);
      break;

    case TDSRRC_CHANNEL_CONFIG_CNF:
      /* First check the validity of Confirmation */
      if(cmd_ptr->cmd.chan_config_cnf.procedure != TDSRRC_PROCEDURE_CCM)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unexpected Channel Config CNF with Proc ID %d",cmd_ptr->cmd.chan_config_cnf.procedure);
        return (next_substate);
      }
    
      if(cmd_ptr->cmd.chan_config_cnf.chan_cfg_status != TDSRRCLLC_CHAN_CFG_SUCCESS)
      {
        /* Channel Config failed - go to Idle substate and send Cell Loss to other procs */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quit Cell Change (Not Camped)- Channel Config failed");
        switch(cmd_ptr->cmd.chan_config_cnf.chan_cfg_status)
        {
          case TDSRRCLLC_CHAN_CFG_FAILURE_PHY_CHL:
          case TDSRRCLLC_CHAN_CFG_FAILURE_LOG_CHL: 
            cell_change_status = TDSRRC_CELL_CHANGE_FAILURE_LOWER_LAYERS;
            break;
        
          case TDSRRCLLC_CHAN_CFG_FAILURE_INCOMPATIBLE_STATE:
            cell_change_status = TDSRRC_CELL_CHANGE_FAILURE_INCOMPATIBLE_STATE;
            break;
        
          default:
            cell_change_status = TDSRRC_CELL_CHANGE_FAILURE_OTHER;
            break;
        }
    
        tdsrrcccm_handle_cell_change_failure(tdsrrc_ccm_cell_camping_info.req_proc_id,
                                           &tdsrrc_ccm_cell_camping_info.curr_cell,
                                           cell_change_status);
        next_substate = TDSRRC_CCM_IDLE;
      }
      else /* Channel Config succedded */
      {
        /* Send a Cell parameter Req to L1 */
        if(tdsrrccsp_send_l1_cmd(TDSRRC_PROCEDURE_CCM, TDSL1_CPHY_CELL_PARM_REQ, &tdsrrc_ccm_cell_camping_info.curr_cell)
          == FAILURE)
        {
          ERR_FATAL("Failed to send L1/LLC cmd %d",TDSL1_CPHY_CELL_PARM_REQ,0,0);
        }
    
        if((rrcmeas_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
        {
          rrcmeas_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CELL_SELECTED_MEAS_REQ;
          rrcmeas_cmd_ptr->cmd.cell_selected_meas_req.gtow_ho_active = FALSE;
          tdsrrc_put_int_cmd(rrcmeas_cmd_ptr);
        }
        else
        {
          ERR_FATAL("Couldn't get internal command buffer",0,0,0);
        }    

        tdsrrcccm_handle_cell_change_success();
    
        /* Set the CCM substate to Idle so as to accept the next Cell Change Req.*/
        next_substate = TDSRRC_CCM_IDLE;
    
    #if (!(defined(T_WINNT)))
        /* Log the state change with QXDM */
        tdsrrcccm_log_cell_info();
    #endif /* (!(defined(T_WINNT))) */
    
      } /* Channel Config succedded */

      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Unexpected Cmd 0x%x in RRC CCM substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_ccm_substate);
      break;

  }

  return(next_substate);

} /* tdsrrcccm_wait_chan_config_handler */



/*===========================================================================

FUNCTION          tdsrrcccm_wait_sib_req_pending_handler

DESCRIPTION       This function is responsible for handling all commands 
                  in TDSRRC_CCM_WAIT_SIB_REQ_PENDING substate.
DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None.

===========================================================================*/
static tdsrrc_ccm_substate_e_type 
tdsrrcccm_wait_sib_req_pending_handler(void)
{
  /* TBD: To be expanded later when CCM has to handle multiple Cell Change
   * Requests simultaneously coming from different procedures.
   */
  return(TDSRRC_CCM_WAIT_SIB_REQ_PENDING);
} /* tdsrrcccm_wait_sib_req_pending_handler */



/*===========================================================================

FUNCTION          tdsrrcccm_wait_sib_handler

DESCRIPTION       This function is responsible for handling all commands 
                  in TDSRRC_CCM_WAIT_SIB substate.
DEPENDENCIES
                  None

RETURN VALUE
                  Next CCM substate.

SIDE EFFECTS
                  TBD: This function checks the current RRC state to decide
                  what actions are needed in terms of RRC state change and RB0
                  reconfig. A race condition could change the state of RRC 
                  between the time a Cell Change Request is sent to CCM and 
                  the time it comes to this point. Such race conditions need
                  to be taken care of in future.
===========================================================================*/
static tdsrrc_ccm_substate_e_type 
tdsrrcccm_wait_sib_handler( tdsrrc_cmd_type  *cmd_ptr )
{
  tdsrrc_ccm_substate_e_type  next_substate = TDSRRC_CCM_WAIT_SIB;
  tdsrrc_state_e_type         current_rrc_state;

  /* Cell selection status to be provided to SIB procedure */
  boolean cell_selection_status = TRUE;

  /* Set the following to TRUE if we can go back to the existing serving
   * cell. This is only used if cell_selection_status (above)
   * is set to FALSE. Set to FALSE if the existing cell is
   * no longer valid and the new cell selection failed. */
  boolean existing_serving_cell_valid = TRUE;
  tdsrrc_cell_change_status_e_type cell_change_status;

  rrc_plmn_identity_type plmn_id;
  unsigned long sib7_timer_val;

  if(tdsrrc_ccm_cell_camping_info.curr_cell.is_valid_nw_sharing)
  {
    plmn_id = tdsrrc_ccm_cell_camping_info.curr_cell.shared_list_of_plmns.common_plmn_id;
  }
  else
  {
    plmn_id = tdsrrc_ccm_cell_camping_info.curr_cell.plmn_id;
  }

  TDSRRC_MSG2(MSG_LEGACY_LOW,"Cmd 0x%x received in CCM substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_ccm_substate);

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_GET_ALL_SIBS_CNF:
      /* First check if SIBs are successfully received */
      if(cmd_ptr->cmd.get_sibs_cnf.status != TDSSIB_EVENT_SUCCEEDED)
      {
        /* SIBs failed - go to Idle substate and send Cell Loss to other procs */
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"Quit Cell Change (Not Camped)- SIBs failed");
        /* Update the cell change status to be sent to CSP */
        switch(cmd_ptr->cmd.get_sibs_cnf.status)
        {
          case TDSSIB_EVENT_FAILED_LOWER_LAYERS:
            cell_change_status = TDSRRC_CELL_CHANGE_FAILURE_LOWER_LAYERS;
            break;
          case TDSSIB_EVENT_FAILED_INCOMPATIBLE_STATE:
            cell_change_status = TDSRRC_CELL_CHANGE_FAILURE_INCOMPATIBLE_STATE;
            break;
          default:
            cell_change_status = TDSRRC_CELL_CHANGE_FAILURE_OTHER;
            break;
        }

        cell_change_status = TDSRRC_CELL_CHANGE_FAILURE_OTHER;

        tdsrrcccm_handle_cell_change_failure(tdsrrc_ccm_cell_camping_info.req_proc_id,
                                           &tdsrrc_ccm_cell_camping_info.curr_cell,
                                           cell_change_status);

        return(TDSRRC_CCM_IDLE);
      }

      /* Indicate the cell selection status to SIB procedure */
      if(FAILURE == tdsrrcsib_change_serving_cell_status(cell_selection_status, existing_serving_cell_valid, 
                                        plmn_id,
                                        tdsrrc_ccm_cell_camping_info.curr_cell.cell_id))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not find PLMN/CellId");
      }

      /* TDSRRCTMR_SIB7_UPDATE_WAIT_TIMER only starts if tds_nv_sib7_exp_time_factor is not 0.
         Only when sib confirm received would set serving cell index.
      */
      if (tds_nv_sib7_exp_time_factor > 0) 
      {
        /* Get default sib7 expiration timer value from sib7 */
        sib7_timer_val = tdsrrcsib_get_sib7_exp_timer_value();

        /* We need to multiply a NV value to start NV controlled sib7 wait timer.
           Default NV value is 1. */
        sib7_timer_val *= tds_nv_sib7_exp_time_factor;
        tdsrrctmr_start_timer(TDSRRCTMR_SIB7_UPDATE_WAIT_TIMER, sib7_timer_val);
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB7_UPDATE_WAIT_TIMER started,timer_val = %d",sib7_timer_val);
      }
      else
      {
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB7_UPDATE_WAIT_TIMER not started,tds_nv_sib7_exp_time_factor = %d",tds_nv_sib7_exp_time_factor);
      }

      /* Check the RRC states under which an RB reconfig/Setup or a state change
       * is needed.
       */
      current_rrc_state = tdsrrc_get_state();
     
      if((current_rrc_state == TDSRRC_STATE_CONNECTING) 
          || (current_rrc_state == TDSRRC_STATE_CELL_FACH && tdsrrc_ccm_cell_camping_info.cell_change_type == TDSRRC_CHANGE_RETURN_TO_SERVICE_AREA) 
          || (current_rrc_state == TDSRRC_STATE_CELL_DCH && tdsrrc_ccm_cell_camping_info.cell_change_type == TDSRRC_CHANGE_CELL_DCH_TO_CELL_FACH))
      {
        /*  for OOS or RLF or being preempted, we should aquire lock here for getting tx resource besides rx resource. */
        if(tdsrrcwrm_acquire_lock_for_pch_to_fach() == FAILURE)
        {           
          tdsrrcccm_handle_cell_change_failure(tdsrrc_ccm_cell_camping_info.req_proc_id,
                                                &tdsrrc_ccm_cell_camping_info.curr_cell,
                                                TDSRRC_CELL_CHANGE_FAILURE_OTHER);
          return(TDSRRC_CCM_IDLE);
        }
        tdrm_retain_lock(TDRM_CLIENT_RRC);
      }

      /* Send LLC a command indicating the new state and a RB reconfig if that
       * applies and wait for LLC to respond back with a CNF.
       */
      tdsrrcccm_send_chan_config_cmd(current_rrc_state,
                                      tdsrrc_ccm_cell_camping_info.cell_change_type);
      /* Channel Config Command sent successfully */
      next_substate = TDSRRC_CCM_WAIT_CHAN_CONFIG_CNF;
      break;

    case TDSRRC_STATE_CHANGE_IND:
      next_substate = tdsrrcccm_update_state_change(cmd_ptr);
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Unexpected Cmd 0x%x in RRC CCM substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_ccm_substate);
      break;
  }

  return(next_substate);

} /* tdsrrcccm_wait_sib_handler */

/*===========================================================================

FUNCTION          tdsrrcccm_idle_handler

DESCRIPTION       This function is responsible for handling all commands 
                  in TDSRRC_CCM_IDLE substate.
DEPENDENCIES
                  None

RETURN VALUE
                  Next CCM substate.

SIDE EFFECTS
                  None.

===========================================================================*/
static tdsrrc_ccm_substate_e_type tdsrrcccm_idle_handler( tdsrrc_cmd_type  *cmd_ptr )
{
  tdsrrc_ccm_substate_e_type  next_substate = TDSRRC_CCM_IDLE;
  tdsrrc_csp_sib_mask_type    sib_mask = 0;
  tdsrrc_cmd_type             *cell_reselection_cmd_ptr;
  tdsrrc_state_e_type         rrc_state;
  tdsrrc_csp_substate_e_type  csp_sstate;

  rrc_plmn_identity_type prev_plmnid, curr_plmnid;
  TDSRRC_MSG2(MSG_LEGACY_LOW,"Cmd 0x%x received in CCM substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_ccm_substate);

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_CELL_CHANGE_REQ:
      if(cmd_ptr->cmd.cell_change_req.req_proc_id >= TDSMAX_NUM_OF_PROCEDURES)
      {
        /* We don't entertain procedures that mask their identity */
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Cell Change Req recvd from Invalid Proc %d",cmd_ptr->cmd.cell_change_req.req_proc_id);
      }
      else /* Request came from a valid Procedure ID */
      {
        rrc_state = tdsrrc_get_state();
        
        if(
            (((tdsrrcllc_get_ordered_config_status() == TDSOC_SET_FOR_CELL_DCH) || 
           ((tdsrrcllc_get_ordered_config_status() != TDSOC_SET_FOR_DCH_FACH_TRANS) && 
            (tdsrrcllc_get_ordered_config_status() != TDSOC_SET_FOR_DCH_CELL_PCH_TRANS) && 
            (tdsrrcllc_get_ordered_config_status() != TDSOC_SET_FOR_DCH_URA_PCH_TRANS)
            && ((tdsrrcllc_get_toc_usage() != TDSTOC_FOR_OOS))
            ))&& (rrc_state == TDSRRC_STATE_CELL_DCH)) 
            ||
            /*
              If OC is set for DCH and reselection is triggered, then drop the cell change request
              >If going to DCH fails , OOS search will anyhow be initiated
              >If going to DCH succeeds, L1 will clear the reselection
            */
            ((tdsrrcllc_get_ordered_config_status() == TDSOC_SET_FOR_CELL_DCH) &&
              (cmd_ptr->cmd.cell_change_req.new_cell_camp_status == TDSRRC_NEW_CELL_SELECTED) &&
              (cmd_ptr->cmd.cell_change_req.cell_change_type == TDSRRC_CHANGE_CELL_RESELECTION)
            )
          )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Incompatible state,drop cell change");
          /* Send a Cell Change CNF with Failure (incompatible state) to CSP */
          if(tdsrrcccm_send_cell_change_cnf(TDSRRC_CELL_CHANGE_FAILURE_INCOMPATIBLE_STATE,
                                          tdsrrc_ccm_cell_camping_info.cell_camp_status,
                                          cmd_ptr->cmd.cell_change_req.req_proc_id,
                                          &tdsrrc_ccm_cell_camping_info.curr_cell)
             == FAILURE)
          {
            ERR_FATAL("Failure to send Cell Change Cnf",0,0,0);
          }
        }
        else
        {
          /* First copy current cell to previous cell since we are ready to accept
           * next cell change request.
           */
          tdsrrc_ccm_cell_camping_info.prev_cell = tdsrrc_ccm_cell_camping_info.curr_cell;
          tdsrrc_ccm_cell_camping_info.prev_cell_access =  tdsrrc_ccm_cell_camping_info.curr_cell_access;
#ifdef FEATURE_DSAC
          TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: Back up of DSAC info during  <ccm_idle_handler> !");
          tdsrrc_ccm_cell_camping_info.prev_dsac_present =  
                                  tdsrrc_ccm_cell_camping_info.curr_dsac_present;

          tdsrrc_ccm_cell_camping_info.prev_cs_cell_access = 
                                  tdsrrc_ccm_cell_camping_info.curr_cs_cell_access;

          tdsrrc_ccm_cell_camping_info.prev_ps_cell_access = 
                                  tdsrrc_ccm_cell_camping_info.curr_ps_cell_access;

#endif   
          /* TBD: Introduce a check on the state in which Cell Change command comes */
          if(cmd_ptr->cmd.cell_change_req.new_cell_camp_status == TDSRRC_CELL_LOSS_NO_NEW_CELL)
          {
            /* Request is for a Cell Loss */
            /* Check the existing status */
            if(tdsrrc_ccm_cell_camping_info.cell_camp_status == TDSRRC_NEW_CELL_SELECTED) 
            {
              /* Existing status is TDSRRC_NEW_CELL_SELECTED */
              tdsrrc_ccm_cell_camping_info.req_proc_id = 
                                        cmd_ptr->cmd.cell_change_req.req_proc_id;
              tdsrrc_ccm_cell_camping_info.cell_camp_status = 
                                        cmd_ptr->cmd.cell_change_req.new_cell_camp_status;
  
              /* TBD: Do we need to indicate to LLC about loss of service - so as to 
               * bring down channels ? Thought for later.
               */     
  
              /* Let us send a New Cell Ind so that those procedures who want to be
               * informed of a loss of cell will get to know.
               */
              if(tdsrrcccm_send_new_cell_ind(TDSRRC_CELL_LOSS_NO_NEW_CELL, 
                                           tdsrrc_ccm_cell_camping_info.req_proc_id,
                                           FALSE, FALSE)
                 == FAILURE)
              {
                ERR_FATAL("Failure to send New Cell Ind",0,0,0);
              }
  
              /* Stop NV controlled sib7 timer and reset global flag */
              tdsrrctmr_stop_timer(TDSRRCTMR_SIB7_UPDATE_WAIT_TIMER);
  
            } /* Existing status is TDSRRC_NEW_CELL_SELECTED */
  
            else /* Existing Status is TDSRRC_CELL_LOSS_NO_NEW_CELL */
            {
              /* This is a request indicating loss of cell. Simply update cell 
               * camping info.
               */
              tdsrrc_ccm_cell_camping_info.req_proc_id = 
                                        cmd_ptr->cmd.cell_change_req.req_proc_id;
              tdsrrc_ccm_cell_camping_info.cell_camp_status = 
                                        cmd_ptr->cmd.cell_change_req.new_cell_camp_status;
  
              /* No need to inform LLC in this case since we were not camped 
               * before also.
               */
  
            } /* Existing Status is TDSRRC_CELL_LOSS_NO_NEW_CELL */
  
          } /* Requested Cell Status == TDSRRC_CELL_LOSS_NO_NEW_CELL */
  
          else if(cmd_ptr->cmd.cell_change_req.new_cell_camp_status == TDSRRC_NEW_CELL_SELECTED)
          {
            csp_sstate = tdsrrccsp_get_csp_substate();

            /* TBD: Set the camping status to Not Camped so as to deter activity by
             * any procedure that cares about the Cell camping status. This status
             * will remain as Not Camped till all things that are needed for cell
             * change are accomplished successfully.
             */
            tdsrrc_ccm_cell_camping_info.req_proc_id = 
                                        cmd_ptr->cmd.cell_change_req.req_proc_id;
            tdsrrc_ccm_cell_camping_info.cell_camp_status = 
                                        cmd_ptr->cmd.cell_change_req.new_cell_camp_status;

            tdsrrc_ccm_cell_camping_info.cell_change_type = 
                                        cmd_ptr->cmd.cell_change_req.cell_change_type;

            /* Initialize the Cell Camping Info with the cmd data */
            tdsrrcccm_copy_curr_cell_camping_info(cmd_ptr);

            tdsrrc_ccm_cell_camping_info.curr_cell.is_valid_nw_sharing = cmd_ptr->cmd.cell_change_req.is_valid_nw_sharing;
            tdsrrc_ccm_cell_camping_info.curr_cell.shared_list_of_plmns.common_plmn_id = cmd_ptr->cmd.cell_change_req.common_plmn_id;
            if(tdsrrc_ccm_cell_camping_info.curr_cell.is_valid_nw_sharing)
            {
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"NWS: Valid NWS common PLMN MCC:%d MNC:%d",TDSRRC_CSP_GET_INT_MCC(tdsrrc_ccm_cell_camping_info.curr_cell.shared_list_of_plmns.common_plmn_id),TDSRRC_CSP_GET_INT_MNC(tdsrrc_ccm_cell_camping_info.curr_cell.shared_list_of_plmns.common_plmn_id));
            }
            if(tdsrrc_ccm_cell_camping_info.prev_cell.is_valid_nw_sharing)
            {
              prev_plmnid = tdsrrc_ccm_cell_camping_info.prev_cell.shared_list_of_plmns.common_plmn_id;
            }
            else
            {
              prev_plmnid = tdsrrc_ccm_cell_camping_info.prev_cell.plmn_id;
            }
          
            if(tdsrrc_ccm_cell_camping_info.curr_cell.is_valid_nw_sharing)
            {
              curr_plmnid = tdsrrc_ccm_cell_camping_info.curr_cell.shared_list_of_plmns.common_plmn_id;
            }
            else
            {
              curr_plmnid = tdsrrc_ccm_cell_camping_info.curr_cell.plmn_id;
            }

            if( (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_RESELECTION_SCAN) && 
                (tdsrrc_bcch_plmns_match(curr_plmnid, prev_plmnid) == TRUE) &&
                (tdsrrc_ccm_cell_camping_info.curr_cell.cell_id == tdsrrc_ccm_cell_camping_info.prev_cell.cell_id) )
            {
              /* Cell Reselection to the same cell - Fail it */
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"Cell Reselection to the same cell- failing reselection");

              tdsrrcsibdb_print_cellid_psc_plmnid_from_sibdb();

              tdsrrcccm_handle_cell_change_failure(tdsrrc_ccm_cell_camping_info.req_proc_id,
                                                 &tdsrrc_ccm_cell_camping_info.curr_cell,
                                                 TDSRRC_CELL_CHANGE_FAILURE_OTHER);
              break;
            }

            if(cmd_ptr->cmd.cell_change_req.cell_change_type == TDSRRC_CHANGE_CELL_RESELECTION)
            {
              /* This check is done to make sure that CCM and CSP substates are in Sync 
               * during reselection
               */
              if(   (csp_sstate == TDSRRCCSP_SUIT_CELL_RESELECT_WT_CAMP)
                 || (csp_sstate == TDSRRCCSP_ACCEPT_CELL_RESELECT_WT_CAMP)
                 || (csp_sstate == TDSRRCCSP_SUIT_CELL_RESELECT_WT_CAMP_PENDING)
                 || (csp_sstate == TDSRRCCSP_ACCEPT_CELL_RESELECT_WT_CAMP_PENDING))
              {
                tdsrrc_ccm_cell_camping_info.status = TDSSTATUS_INVALID_CELL_RESELECTION;
  
  
                if((cell_reselection_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
                {
                  /* Fill the TDSRRC_CELL_RESELECTION_REQ to be sent to LLC asking it to
                  configure the lower layers */
                  cell_reselection_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CELL_RESELECTION_REQ;
                  cell_reselection_cmd_ptr->cmd.cell_resel_req.freq = cmd_ptr->cmd.cell_change_req.freq;
                  cell_reselection_cmd_ptr->cmd.cell_resel_req.cpid = cmd_ptr->cmd.cell_change_req.cpid;
                  if(cmd_ptr->cmd.cell_change_req.is_valid_nw_sharing)
                  {
                    cell_reselection_cmd_ptr->cmd.cell_resel_req.plmn_id = cmd_ptr->cmd.cell_change_req.common_plmn_id;
                  }
                  else
                  {
                    cell_reselection_cmd_ptr->cmd.cell_resel_req.plmn_id = cmd_ptr->cmd.cell_change_req.plmn_id;
                  }
                  cell_reselection_cmd_ptr->cmd.cell_resel_req.cell_id = cmd_ptr->cmd.cell_change_req.cell_id;
                }
                else /* No cmd buffer for RRC Internal command */
                {
                  ERR_FATAL("Unable to get Int Cmd buf for RRC Cmd %d",TDSRRC_CELL_RESELECTION_REQ,0,0);
                }
  
                /* Send the Internal RRC Command - LLC should directly handle it */
                tdsrrc_put_int_cmd( cell_reselection_cmd_ptr );
  
                /* Wait for TDSRRC_CELL_RESELECTION_CNF from LLC */
                next_substate = TDSRRC_CCM_WAIT_CELL_RESELECTION_CNF; 
              }
              else
              {
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"Ignoring TDSRRC_CELL_CHANGE_REQ (for resel) in CCM when CSP st %d",csp_sstate); 
                tdsrrcccm_update_with_cell_loss();
              }
            }
            else
            {
              tdsrrc_ccm_cell_camping_info.status = TDSSTATUS_INVALID_CELL_TO_BE_CAMPED;

              /* Send the ASET Update Command to L1 */
              if(tdsrrccsp_send_l1_cmd(TDSRRC_PROCEDURE_CCM, TDSL1_CPHY_CELL_TRANSITION_REQ, 
                                     &tdsrrc_ccm_cell_camping_info.curr_cell) 
                 == FAILURE)
              {
                ERR_FATAL("Failed to send L1/LLC cmd %d",TDSL1_CPHY_CELL_TRANSITION_REQ,0,0);
              }

              /* Now send a Command to SIB to get all SIBs */
              if(tdsrrccsp_send_sib_cmd(TDSRRC_GET_ALL_SIBS_REQ, sib_mask, 
                                        &tdsrrc_ccm_cell_camping_info.curr_cell, TRUE)
                   == FAILURE)
              {
                ERR_FATAL("Failed to send RRC cmd %d",TDSRRC_GET_ALL_SIBS_REQ,0,0);
              }
              next_substate = TDSRRC_CCM_WAIT_SIB;        
            }   
          } /* Requested Cell Status == TDSRRC_NEW_CELL_SELECTED */

          else /* Unexpected cell status */
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid cell status %d in Cell Change Req - Ignored",cmd_ptr->cmd.cell_change_req.new_cell_camp_status);
            /* Do not send a CNF back - Ignore this Cell Change Req. */
          }
        }
      }/* Request came from valid Procedure ID */

      break;

    case TDSRRC_STATE_CHANGE_IND:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"State change in CCM_IDLE,ignore it");
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Unexpected Cmd 0x%x in RRC CCM substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrc_ccm_substate);
      break;
  }

  return(next_substate);

} /* tdsrrcccm_idle_handler */


/*===========================================================================
FUNCTION          tdsrrcccm_update_cell_camped_status

DESCRIPTION       This function is responsible for putting the 
                  "Cell camp status" to TDSRRC_NEW_CELL_SELECTED
                  when that status type is passed as a parameter. 
                  Also, new cell indication is sent to RCE if it was waiting
                  for UE to camp on

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void tdsrrcccm_update_cell_camped_status(tdsrrc_new_cell_status_e_type status)
{
  if(tdsrrc_ccm_substate == TDSRRC_CCM_IDLE)
  {  
    if(status == TDSRRC_NEW_CELL_SELECTED)
    {
      if(tdsrrc_ccm_cell_camping_info.cell_camp_status == TDSRRC_CELL_LOSS_NO_NEW_CELL)
      {
        tdsrrc_ccm_cell_camping_info.req_proc_id = TDSRRC_PROCEDURE_CSP;
        tdsrrc_ccm_cell_camping_info.cell_camp_status = status;

        /* No need to inform LLC in this case */
        if(tdsrrc_ccm_cell_camping_info.rce_found_status_not_camping)
        {
          /* Send an Indication ONLY when either the new cell is different
           * than the last camped on cell OR if RCE wants an Indication.
           * In this case, the cell remains the same, so the check is only
           * on RCE wanting an Indication.
           */
          /* We don't want the procedure initiating this change to get an IND 
           * It will get a CNF and an extra IND might be confusing.
           */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send New Cell Ind to RCE");
          if(tdsrrcccm_send_new_cell_ind(TDSRRC_NEW_CELL_SELECTED, 
                                       TDSRRC_PROCEDURE_CSP,
                                       TRUE,
                                       tdsrrc_ccm_cell_camping_info.rce_found_status_not_camping)
             == FAILURE)
          {
            ERR_FATAL("Failure to send New Cell Ind",0,0,0);
          }

          tdsrrc_ccm_cell_camping_info.rce_found_status_not_camping = FALSE;

        }  /* If inform_rce_procedure */
      } /* If camping status is "Cell loss Idle resel in prog" */
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"NEW_CELL_SELECTED,curr camp %d ",tdsrrc_ccm_cell_camping_info.cell_camp_status);
      }
    } /* New Cell Selected */
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR Status type %d not recognized",status);
    }
  } /* CCM is in Idle substate */
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR update cell status func called when CCM is not in idle %d",tdsrrc_ccm_substate);
  }
} /* tdsrrcccm_update_cell_camped_status */


#ifdef FEATURE_INTERRAT_HANDOVER_G2TD
/*===========================================================================
FUNCTION          tdsrrcccm_update_for_g2td_handover

DESCRIPTION       This function will update the CCM camping information for
                  GSM to TDS handover.

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void
tdsrrcccm_update_for_g2td_handover(tdsrrc_csp_acq_entry_type * acq_entry_ptr)
{
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"TDS_G2TD_HHO_CCM: (%d,%d)",acq_entry_ptr->freq,acq_entry_ptr->cpid);

  tdsrrc_ccm_cell_camping_info.status = TDSSTATUS_VALID;
  tdsrrc_ccm_cell_camping_info.cell_camp_status = TDSRRC_NEW_CELL_SELECTED;
  tdsrrc_ccm_cell_camping_info.cell_change_type = TDSRRC_CHANGE_CELL_SELECTION;
  tdsrrc_ccm_cell_camping_info.curr_cell.cpid = acq_entry_ptr->cpid;
  tdsrrc_ccm_cell_camping_info.curr_cell.freq = acq_entry_ptr->freq;
  tdsrrc_ccm_cell_camping_info.curr_cell.plmn_id = acq_entry_ptr->plmn_id;
  tdsrrc_ccm_cell_camping_info.is_curr_cell_id_valid = FALSE;
  tdsrrc_ccm_cell_camping_info.curr_cell.cell_id = 0;
  tdsrrc_ccm_cell_camping_info.rce_found_status_not_camping = FALSE;
  tdsrrc_ccm_cell_camping_info.curr_cell_access = MM_AS_ACCESS_ALL_CALLS;
#ifdef FEATURE_DSAC
  tdsrrc_ccm_cell_camping_info.curr_cs_cell_access = MM_AS_ACCESS_ALL_CALLS;
  tdsrrc_ccm_cell_camping_info.curr_ps_cell_access = MM_AS_ACCESS_ALL_CALLS;
  tdsrrc_ccm_cell_camping_info.curr_dsac_present = TRUE;
#endif
  tdsrrc_ccm_substate = TDSRRC_CCM_IDLE;

} /* tdsrrcccm_update_for_g2td_handover */
#endif


/*===========================================================================
FUNCTION          tdsrrcccm_update_for_inter_freq_hard_handover

DESCRIPTION       This function will update the CCM camping information with
                  the new frequency and scrambling code after a successful
                  inter-frequency hard handover.

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void
tdsrrcccm_update_for_inter_freq_hard_handover(tdsrrc_freq_type freq, boolean cpid_present, tdsrrc_cpid_type cpid)
{
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"TDS_HHO_CCM: from (%d,%d)",tdsrrc_ccm_cell_camping_info.curr_cell.freq,tdsrrc_ccm_cell_camping_info.curr_cell.cpid);
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"TDS_HHO_CCM: to (%d,%d)",freq,cpid);
  tdsrrc_cs_domain_error.mo_error_count = 0;
  tdsrrc_cs_domain_error.mt_error_count = 0;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"cs_domain_error: reset tdsrrc_cs_domain_error");   
  if (cpid_present)
  {
    tdsrrc_ccm_cell_camping_info.curr_cell.cpid = cpid;
  }
  tdsrrc_ccm_cell_camping_info.curr_cell.freq = freq;

  tdsrrc_csp_int_data.curr_acq_entry.freq     = freq;
  if (cpid_present)
  {
    tdsrrc_csp_int_data.curr_acq_entry.cpid = cpid;
  }
  tdsrrc_csp_int_data.curr_acq_entry.status_mask = 0;

  tdsrrc_csp_int_data.curr_acq_entry.band_class = tdsrrc_get_frequency_band(freq);
  TDSRRC_MSG2(MSG_LEGACY_MED,"Setting band_class 0x%x for freq %d",tdsrrc_csp_int_data.curr_acq_entry.band_class,freq);

  tdsrrc_csp_int_data.curr_acq_entry.is_valid_nw_sharing = FALSE;

  /* Store the Current Acq. entry in Acq. database. */
  if( tdsrrccsp_add_acq_entry(&tdsrrc_csp_int_data.curr_acq_entry) == FAILURE)
  {
    ERR_FATAL("Failure adding acq. entry",0,0,0);
  }

} /* tdsrrcccm_update_for_inter_freq_hard_handover */



/*===========================================================================

FUNCTION          tdsrrcccm_cell_change_manager

DESCRIPTION       This function is responsible for handling all commands 
                  relevant to Cell Change Manager.

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None.

===========================================================================*/
void  tdsrrcccm_cell_change_manager( tdsrrc_cmd_type  *cmd_ptr )
{
  tdsrrc_event_ccm_state_info_type ccm_state_event;
  tdsrrc_ccm_substate_e_type current_ccm_state = tdsrrc_ccm_substate;
  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_MODE_CHANGE_IND)
  {
    if(cmd_ptr->cmd.mode_change_ind.to_mode == TDSRRC_MODE_INACTIVE )
    {
      /* CCM goes back to the initial sub-state and the CCM database is cleared.
       * The status valid is set to FALSE till a Cell Change Req is processed.
       */
      tdsrrc_ccm_cell_camping_info.status = TDSSTATUS_INVALID_CELL_TO_BE_CAMPED;
      tdsrrc_ccm_cell_camping_info.cell_camp_status = TDSRRC_CELL_LOSS_NO_NEW_CELL;
    
      /* Set to FALSE, the boolean that indicates that RCE found status as "Not Camping"*/
      tdsrrc_ccm_cell_camping_info.rce_found_status_not_camping = FALSE;
      /* Set the Cell change requesting procedure ID to Invalid value */
      tdsrrc_ccm_cell_camping_info.req_proc_id = TDSMAX_NUM_OF_PROCEDURES;
    
      /* Initialize the state of RRC CCM here */
      tdsrrc_ccm_substate = TDSRRC_CCM_IDLE;
    }
    else
    {
      /* TBD - Processing for other mode changes */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Mode change to %d not supported",cmd_ptr->cmd.mode_change_ind.to_mode);
    }
  }
  else
  {
    switch(tdsrrc_ccm_substate)
    {
      case TDSRRC_CCM_IDLE:
        tdsrrc_ccm_substate = tdsrrcccm_idle_handler(cmd_ptr);
        break;
      case TDSRRC_CCM_WAIT_SIB:
        tdsrrc_ccm_substate = tdsrrcccm_wait_sib_handler(cmd_ptr);
        break;
      case TDSRRC_CCM_WAIT_SIB_REQ_PENDING:
        tdsrrc_ccm_substate = tdsrrcccm_wait_sib_req_pending_handler();
        break;
      case TDSRRC_CCM_WAIT_CHAN_CONFIG_CNF:
        tdsrrc_ccm_substate = tdsrrcccm_wait_chan_config_handler(cmd_ptr);
        break;
      case TDSRRC_CCM_WAIT_CELL_RESELECTION_CNF:
        tdsrrc_ccm_substate = tdsrrcccm_wait_cell_reselection_handler(cmd_ptr);
        break;
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RRC CCM substate %d",tdsrrc_ccm_substate);
        break;
    }
  }

  ccm_state_event.ccm_sub_state = (uint8) tdsrrc_ccm_substate;

  event_report_payload(EVENT_TDSCDMA_RRC_CCM_SUBSTATE, 
                       sizeof(tdsrrc_event_ccm_state_info_type), 
                       (void *)&ccm_state_event);
  
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"TDS CCM State changed(%d): %d => %d ",
           (current_ccm_state != tdsrrc_ccm_substate),
           current_ccm_state,
           ccm_state_event.ccm_sub_state);

} /* tdsrrcccm_cell_change_manager */


/*===========================================================================

FUNCTION          tdsrrcccm_force_init

DESCRIPTION       This function is responsible for initializing data 
                  structures associated with Cell Change Manager.

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None.

NOTE
                  This function SHALL NOT send any cmd to any other procedure.
===========================================================================*/
void tdsrrcccm_force_init( void )
{
  /* The status valid is set to FALSE till a Cell Change Req is processed.
   */
  tdsrrc_ccm_cell_camping_info.status = TDSSTATUS_INVALID_CELL_TO_BE_CAMPED;
  tdsrrc_ccm_cell_camping_info.cell_camp_status = TDSRRC_CELL_LOSS_NO_NEW_CELL;

  /* Set to FALSE, the boolean that indicates that RCE found status as "Not Camping"*/
  tdsrrc_ccm_cell_camping_info.rce_found_status_not_camping = FALSE;
  /* Set the Cell change requesting procedure ID to Invalid value */
  tdsrrc_ccm_cell_camping_info.req_proc_id = TDSMAX_NUM_OF_PROCEDURES;

  /* Initialize the state of RRC CCM here */
  tdsrrc_ccm_substate = TDSRRC_CCM_IDLE;

} /* tdsrrcccm_force_init */

/*===========================================================================

FUNCTION          rrccm_update_service_info

DESCRIPTION       This function is responsible for updatintng service access
                            information OTA message sent to MM 

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None.

NOTE
                  This function SHALL NOT send any cmd to any other procedure.
===========================================================================*/
void tdsrrcccm_update_service_info(mm_cmd_type *service_info_ota_ptr)
{
  rrc_plmn_identity_type plmn_id;
  tdsrrc_int_cell_id_type cell_id;

  /* Fill in the service status */
  if ((tdsrrcccm_get_serv_plmn_cellid(&cell_id, &plmn_id) == TDSRRC_CELL_LOSS_NO_NEW_CELL))
  {
    service_info_ota_ptr->cmd.rrc_service_ind.service_status = MM_AS_NO_SERVICE;
  }
  else
  {
    service_info_ota_ptr->cmd.rrc_service_ind.service_status = MM_AS_SERVICE_AVAILABLE;
  }

  if(service_info_ota_ptr->cmd.rrc_service_ind.service_status== MM_AS_SERVICE_AVAILABLE)
  {
    /* Fill the cell access */
    service_info_ota_ptr->cmd.rrc_service_ind.service_info.cell_access =
                                                            tdsrrc_csp_int_data.curr_cell_access_status;
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Curr Cell Access Info= %d  Prev Cell Access Info= %d",service_info_ota_ptr->cmd.rrc_service_ind.service_info.cell_access,tdsrrc_ccm_cell_camping_info.prev_cell_access);
#ifdef FEATURE_DSAC
    if(tdsrrc_csp_int_data.is_dsac_valid == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC  is present");
      service_info_ota_ptr->cmd.rrc_service_ind.service_info.dsac_present  =  
                                                tdsrrc_csp_int_data.is_dsac_valid;
      service_info_ota_ptr->cmd.rrc_service_ind.service_info.cs_cell_access = 
                                                tdsrrc_csp_int_data.curr_cs_cell_access;
      service_info_ota_ptr->cmd.rrc_service_ind.service_info.ps_cell_access = 
                                                 tdsrrc_csp_int_data.curr_ps_cell_access;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: DSAC, CS and PS cell access values:  %d,  %d, %d", 
                          service_info_ota_ptr->cmd.rrc_service_ind.service_info.dsac_present,
                          service_info_ota_ptr->cmd.rrc_service_ind.service_info.cs_cell_access,
                          service_info_ota_ptr->cmd.rrc_service_ind.service_info.ps_cell_access);
    }
    else
    {
       TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC info is not present");
       service_info_ota_ptr->cmd.rrc_service_ind.service_info.dsac_present = tdsrrc_csp_int_data.is_dsac_valid;
       service_info_ota_ptr->cmd.rrc_service_ind.service_info.cell_access = tdsrrc_csp_int_data.curr_cell_access_status;
    }
    if(tdsrrc_csp_int_data.is_dsac_valid == TRUE) 
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC info is valid for CCM");
      tdsrrc_ccm_cell_camping_info.curr_dsac_present = tdsrrc_csp_int_data.is_dsac_valid;
      tdsrrc_ccm_cell_camping_info.curr_cs_cell_access =   
                                  service_info_ota_ptr->cmd.rrc_service_ind.service_info.cs_cell_access;
      tdsrrc_ccm_cell_camping_info.curr_ps_cell_access =   
                                 service_info_ota_ptr->cmd.rrc_service_ind.service_info.ps_cell_access;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: DSAC, CS and PS A/C values in ccm cell camp info %d,  %d, %d", 
                               tdsrrc_ccm_cell_camping_info.curr_dsac_present,
                               tdsrrc_ccm_cell_camping_info.curr_cs_cell_access,
                               tdsrrc_ccm_cell_camping_info.curr_ps_cell_access);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC info is not valid for CCM");
      tdsrrc_ccm_cell_camping_info.curr_dsac_present = tdsrrc_csp_int_data.is_dsac_valid;
    }
#endif
  }
}
/*===========================================================================

FUNCTION          tdsrrcccm_init_cell_change_manager

DESCRIPTION       This function is responsible for initializing data 
                  structures associated with Cell Change Manager.

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None.

NOTE
                  This function SHALL NOT send any cmd to any other procedure.
===========================================================================*/
void tdsrrcccm_init_cell_change_manager( void )
{
  /* The status valid is set to FALSE till a Cell Change Req is processed.
   */
  tdsrrc_ccm_cell_camping_info.status = TDSSTATUS_INVALID_CELL_TO_BE_CAMPED;
  tdsrrc_ccm_cell_camping_info.cell_camp_status = TDSRRC_CELL_LOSS_NO_NEW_CELL;

  /* Set to FALSE, the boolean that indicates that RCE found status as "Not Camping"*/
  tdsrrc_ccm_cell_camping_info.rce_found_status_not_camping = FALSE;
  /* Set the Cell change requesting procedure ID to Invalid value */
  tdsrrc_ccm_cell_camping_info.req_proc_id = TDSMAX_NUM_OF_PROCEDURES;

  /* Initialize the state of RRC CCM here */
  tdsrrc_ccm_substate = TDSRRC_CCM_IDLE;

  tdsrrc_ccm_cell_camping_info.is_curr_cell_id_valid = FALSE;

  /* Register with the State Change Manager for all state transitions 
   * from any state to Idle Disconnected and DCH states.
   */
  tdsrrcscmgr_register_for_scn
  (
    TDSRRC_PROCEDURE_CCM,   
    TDSRRC_STATE_WILDCARD,  
    TDSRRC_STATE_DISCONNECTED
  );

  tdsrrcscmgr_register_for_scn
  (
    TDSRRC_PROCEDURE_CCM,   
    TDSRRC_STATE_WILDCARD,  
    TDSRRC_STATE_CELL_DCH
  );

  tdsrrcscmgr_register_for_scn
  (
    TDSRRC_PROCEDURE_CCM,
    TDSRRC_STATE_CONNECTING,
    TDSRRC_STATE_CELL_FACH
  );

} /* tdsrrcccm_init_cell_change_manager */

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
/*===========================================================================

FUNCTION          RRC_CCM_PROPAGATE_NEW_CELL_TO_WAITING_PROC

DESCRIPTION       This function is responsible for sending the NEW_CELL_IND to the
                  procedure waiting for it.
                  
DEPENDENCIES
                  None

RETURN VALUE
                  None
SIDE EFFECTS
                  None
===========================================================================*/
void tdsrrcccm_propagate_new_cell_to_waiting_proc
(
  tdsrrc_proc_e_type proc_id,
  boolean cell_same_as_before,
  boolean inform_procedure_rce
)
{
  tdsrrc_cmd_type   *cmd_ptr;
  /* Send New Cell Ind */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    cmd_ptr->cmd_hdr.cmd_id = TDSRRC_NEW_CELL_IND;
    cmd_ptr->cmd.new_cell_ind.new_cell_status = TDSRRC_NEW_CELL_SELECTED;
    cmd_ptr->cmd.new_cell_ind.no_ind_proc_id = proc_id;
    cmd_ptr->cmd.new_cell_ind.cell_same_as_before = cell_same_as_before;
    cmd_ptr->cmd.new_cell_ind.inform_procedure_rce = inform_procedure_rce;

    /* Send the Internal cmd to RRC */
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Post NEW_CELL_IND %d, %d, %d", TDSRRC_NEW_CELL_IND,
                                            cell_same_as_before, 
                                            inform_procedure_rce
                                            );

    switch (proc_id)
    {
      case TDSRRC_PROCEDURE_RBE:
        tdsrrcrbe_procedure_event_handler( cmd_ptr );
        break;

      case TDSRRC_PROCEDURE_RBRC:
        tdsrrcrbrc_procedure_event_handler( cmd_ptr );
        break;

      case TDSRRC_PROCEDURE_TCR:
        tdsrrctcrc_procedure_event_handler( cmd_ptr );
        break;

      case TDSRRC_PROCEDURE_PCR:
        tdsrrcpcrc_procedure_event_handler( cmd_ptr );
        break;

      case TDSRRC_PROCEDURE_RBR:
        tdsrrcrbr_procedure_event_handler( cmd_ptr );
        break;

      case TDSRRC_PROCEDURE_CU:
        tdsrrccu_procedure_event_handler( cmd_ptr );
        break;  

      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"This proc is not expected : %d",proc_id);
        break;
    }
  }
  else
  {
    ERR_FATAL("Failure to get Int Cmd buf",0,0,0);
  }
  tdsrrc_free_cmd_buf( cmd_ptr );
}
#endif /* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU */

/*===========================================================================

FUNCTION          RRC_CCM_GET_CURR_CAMPED_COMMON_PLMN

DESCRIPTION       This function is responsible for updating plmn_id with common PLMN_ID if curr cell is shared

DEPENDENCIES      NONE

RETURN VALUE
                  TRUE      If Curr cell is shared
                  FALSE     Otherwise

SIDE EFFECTS      None.
===========================================================================*/
boolean tdsrrcccm_get_curr_camped_common_plmn
(
  rrc_plmn_identity_type *plmn_id
)
{
  if(tdsrrc_ccm_cell_camping_info.curr_cell.is_valid_nw_sharing)
  {
    *plmn_id = tdsrrc_ccm_cell_camping_info.curr_cell.shared_list_of_plmns.common_plmn_id;
  }
  return tdsrrc_ccm_cell_camping_info.curr_cell.is_valid_nw_sharing;
}
/*===========================================================================

FUNCTION          tdsrrcccm_backup_service_ind

DESCRIPTION     This function is responsible for backup service ind from RRC_SERVICE_IND, 
                       RRC_SERVICE_IND_FROM_OTA_MSG, or RRC_SERVICE_CNF

DEPENDENCIES
                  
RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/

void tdsrrcccm_backup_service_ind(rrc_to_mm_sink_e_type  cmd_id, 
                                               mm_cmd_type *mm_buf_ptr)
{
  
   /* Backup info for service ind */
   if((cmd_id == RRC_SERVICE_IND) || (cmd_id == RRC_SERVICE_IND_FROM_OTA_MSG))
   {
     memscpy(&tdsrrc_service_ind_backup, sizeof(rrc_service_ind_type), 
             &(mm_buf_ptr->cmd.rrc_service_ind), sizeof(rrc_service_ind_type));
     TDSRRC_MSG1(MSG_LEGACY_HIGH,"Backup service ind for %d",cmd_id);
   }
   else if (cmd_id == RRC_SERVICE_CNF)
   {
     memscpy(&(tdsrrc_service_ind_backup.service_status), sizeof(mm_as_service_state_T), 
             &(mm_buf_ptr->cmd.rrc_service_cnf.service_status), sizeof(mm_as_service_state_T));
     memscpy(&(tdsrrc_service_ind_backup.service_info), sizeof(rrc_service_info_type), 
             &(mm_buf_ptr->cmd.rrc_service_cnf.service_info), sizeof(rrc_service_info_type));
     memscpy(&(tdsrrc_service_ind_backup.active_band), sizeof(sys_band_mask_e_type), 
             &(mm_buf_ptr->cmd.rrc_service_cnf.active_band), sizeof(sys_band_mask_e_type));

     tdsrrc_service_ind_backup.is_cell_id_valid =mm_buf_ptr->cmd.rrc_service_cnf.is_cell_id_valid; 
     tdsrrc_service_ind_backup.cell_id =mm_buf_ptr->cmd.rrc_service_cnf.cell_id; 
     TDSRRC_MSG1(MSG_LEGACY_HIGH,"Backup service ind for %d",cmd_id);
   }
   else
   {
     TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid CMD for backup service ind%d",cmd_id);
   }
}
